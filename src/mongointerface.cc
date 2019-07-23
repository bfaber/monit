#include <sstream>

#include "mongointerface.h"

//TODO: make this a singleton
MongoInterface::MongoInterface(std::string host, int port, std::string db) : dbName(db) {

    
    // if there are issues with this, might have to rebuild the driver:
    // cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=NO
    mongoc_init (); //only once at beginning of program

    std::stringstream ss;
    ss << "mongodb://" << host << ":" << port;    
    mongoUri = ss.str();
}

MongoInterface::~MongoInterface() {
    mongoc_cleanup();
}

// This will be a transaction style commit of many records to many collections in one
// network hop.  This is the most efficient way to batch insert to mongo.
// most code straight from:
//  http://mongoc.org/libmongoc/current/mongoc_transaction_opt_t.html

// recs will be a list of Records which themselves wrap a list of bson_t docs* to be
// inserted to a given collection.  Each Record corresponds to a different collection.
int MongoInterface::insertRecords(std::vector<Record*> &records) {
    
    int exit_code = EXIT_FAILURE;

    mongoc_client_t *client = NULL;
    mongoc_database_t *database = NULL;
    mongoc_collection_t *collection = NULL;
    mongoc_client_session_t *session = NULL;
    mongoc_session_opt_t *session_opts = NULL;
    mongoc_transaction_opt_t *default_txn_opts = NULL;
    mongoc_transaction_opt_t *txn_opts = NULL;
    mongoc_read_concern_t *read_concern = NULL;
    mongoc_write_concern_t *write_concern = NULL;
    const char *uri_string = "mongodb://127.0.0.1/?appname=transaction-example";
    mongoc_uri_t *uri;
    bson_error_t error;
    bson_t *doc = NULL;
    bson_t *insert_opts = NULL;
    int32_t i;
    int64_t start;
    bson_t reply = BSON_INITIALIZER;
    char *reply_json;
    bool r;



    if (mongoUri.size() > 0) {
	uri_string = mongoUri.c_str();
    }

    uri = mongoc_uri_new_with_error (uri_string, &error);
    if (!uri) {
	MONGOC_ERROR ("failed to parse URI: %s\n"
		      "error message:       %s\n",
		      uri_string,
		      error.message);
	goto done;
    }

    client = mongoc_client_new_from_uri (uri);
    if (!client) {
	goto done;
    }

    mongoc_client_set_error_api (client, 2);
    database = mongoc_client_get_database (client, dbName.c_str());


    /* a transaction's read preferences, read concern, and write concern can be
     * set on the client, on the default transaction options, or when starting
     * the transaction. for the sake of this example, set read concern on the
     * default transaction options. */
    default_txn_opts = mongoc_transaction_opts_new ();
    read_concern = mongoc_read_concern_new ();
    mongoc_read_concern_set_level (read_concern, "snapshot");
    mongoc_transaction_opts_set_read_concern (default_txn_opts, read_concern);
    session_opts = mongoc_session_opts_new ();
    mongoc_session_opts_set_default_transaction_opts (session_opts,
						      default_txn_opts);

    session = mongoc_client_start_session (client, session_opts, &error);
    if (!session) {
	MONGOC_ERROR ("Failed to start session: %s", error.message);
	goto done;
    }

    /* in this example, set write concern when starting the transaction */
    txn_opts = mongoc_transaction_opts_new ();
    write_concern = mongoc_write_concern_new ();
    mongoc_write_concern_set_wmajority (write_concern, 1000 /* wtimeout */);
    mongoc_transaction_opts_set_write_concern (txn_opts, write_concern);

    insert_opts = bson_new ();
    if (!mongoc_client_session_append (session, insert_opts, &error)) {
	MONGOC_ERROR ("Could not add session to opts: %s", error.message);
	goto done;
    }

 retry_transaction:
    r = mongoc_client_session_start_transaction (session, txn_opts, &error);
    if (!r) {
	MONGOC_ERROR ("Failed to start transaction: %s", error.message);
	goto done;
    }

    // for each Record(ie collection), insert the many documents per.
    for (auto *record : records) {

	bson_destroy (&reply);
	/* inserting into a nonexistent collection normally creates it, but a
	 * collection can't be created in a transaction; create it now */
	const char* collName = record->getCollectionName().c_str();
	mongoc_collection_t *collection =
	    mongoc_database_create_collection (database,
					       collName,
					       NULL,
					       &error);

	if (!collection) {
	    /* code 48 is NamespaceExists, see error_codes.err in mongodb source */
	    if (error.code == 48) {
		collection = mongoc_database_get_collection (database, collName);
	    } else {
		MONGOC_ERROR ("Failed to create collection: %s", error.message);
		goto done;
	    }
	}


	//const bson_t** docs = const_cast<const bson_t**>(record->getDocs());
	const bson_t** docs = record->getDocs();
	r = mongoc_collection_insert_many ( collection,
					    docs,
					    record->size(),
					    insert_opts,
					    &reply,
					    &error);
	//	r = mongoc_collection_insert_one (
	//collection, doc, insert_opts, &reply, &error);

	bson_destroy (doc);

	if (!r) {
	    MONGOC_ERROR ("Insert failed: %s", error.message);
	    mongoc_client_session_abort_transaction (session, NULL);

	    /* a network error, primary failover, or other temporary error in a
	     * transaction includes {"errorLabels": ["TransientTransactionError"]},
	     * meaning that trying the entire transaction again may succeed
	     */
	    if (mongoc_error_has_label (&reply, "TransientTransactionError")) {
		goto retry_transaction;
	    }

	    goto done;
	}

	reply_json = bson_as_json (&reply, NULL);
	printf ("%s\n", reply_json);
	bson_free (reply_json);
    }

    /* in case of transient errors, retry for 5 seconds to commit transaction */
    start = bson_get_monotonic_time ();
    while (bson_get_monotonic_time () - start < 5 * 1000 * 1000) {
	bson_destroy (&reply);
	r = mongoc_client_session_commit_transaction (session, &reply, &error);
	if (r) {
	    /* success */
	    break;
	} else {
	    MONGOC_ERROR ("Warning: commit failed: %s", error.message);
	    if (mongoc_error_has_label (&reply, "TransientTransactionError")) {
		goto retry_transaction;
	    } else if (mongoc_error_has_label (&reply,
					       "UnknownTransactionCommitResult")) {
		/* try again to commit */
		continue;
	    }

	    /* unrecoverable error trying to commit */
	    break;
	}
    }

    exit_code = EXIT_SUCCESS;

 done:
    bson_destroy (&reply);
    bson_destroy (insert_opts);
    mongoc_write_concern_destroy (write_concern);
    mongoc_read_concern_destroy (read_concern);
    mongoc_transaction_opts_destroy (txn_opts);
    mongoc_transaction_opts_destroy (default_txn_opts);
    mongoc_client_session_destroy (session);
    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    mongoc_uri_destroy (uri);
    mongoc_client_destroy (client);

    mongoc_cleanup ();

    return exit_code;
    
}

// TODO: would love to break this out, but the cleanup prevents easy refactor
std::vector<ConfigItem*>* MongoInterface::getConfigs(std::string configCollName) {

    bson_error_t error;

    mongoc_uri_t *uri = mongoc_uri_new_with_error (mongoUri.c_str(), &error);
    if (!uri) {
	fprintf (stderr,
		 "failed to parse URI: %s\n"
		 "error message:       %s\n",
		 mongoUri.c_str(),
		 error.message);
	return nullptr;
    }

    mongoc_client_t *client = mongoc_client_new_from_uri (uri);
    if (!client) {
	fprintf (stderr, "failed to get mongoc client obj\n");
	return nullptr;
    }

    mongoc_client_set_error_api (client, 2);

    mongoc_collection_t *collection = mongoc_client_get_collection (client, dbName.c_str(), configCollName.c_str());
    printf("pulling config from db %s\n", dbName.c_str());
    bson_t query;    
    bson_init (&query);
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection,
							       &query,
							       NULL, /*additional opts*/
							       NULL);/*readprefs*/
    
    std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
    const bson_t *doc;
    char *str = nullptr;
    while (mongoc_cursor_next (cursor, &doc)) {

	//str = bson_as_canonical_extended_json (doc, NULL);
	// unpack the bson without intermediate json... unnecessary
	auto *configItem = parseConfigFromBson(doc);
	if(configItem != nullptr) {	    
	    configs->push_back(configItem);
	}
	else {
	    const char* str = bson_as_canonical_extended_json(doc, NULL);
	    fprintf(stderr, "Failed to create configItem - json rep: %s\n", str);
	}
    }

    bson_free(str);
    
    if (mongoc_cursor_error (cursor, &error)) {
	fprintf (stderr, "Cursor Failure: %s\n", error.message);
	return nullptr; // EXIT_FAILURE
    }

    bson_destroy (&query);
    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);
    mongoc_uri_destroy (uri);
    mongoc_client_destroy (client);

    return configs;   // EXIT_SUCCESS // mongoc return enums?
}

/**
 * document must have certain values or nullptr will be returned.
 */
ConfigItem* MongoInterface::parseConfigFromBson(const bson_t *doc) {
    auto ci = new ConfigItem();
    bson_iter_t iter;
    if (bson_iter_init (&iter, doc)) {
	while (bson_iter_next (&iter)) {
	    printf ("Found element key: \"%s\"\n", bson_iter_key (&iter));
	    // now not sure what key will be returned.
	    // maybe I can pass in a struct instead to the bson lib?
	    // i wish!
	    const char* key = bson_iter_key(&iter);
	    if(strcmp(key, "matchItemName") == 0) {
		ci->setName(bson_iter_utf8(&iter, NULL));
	    }
	    else if(strcmp(key, "regex") == 0) {
		ci->setRegex(bson_iter_utf8(&iter, NULL));
	    }
	    else if(strcmp(key, "filename") == 0) {
		ci->setFileName(bson_iter_utf8(&iter, NULL));
	    }
	    else if(strcmp(key, "csv") == 0) {
		ci->setCsv(bson_iter_utf8(&iter, NULL));
	    }
	    else if(strcmp(key, "collectionName") == 0) {
		ci->setCollectionName(bson_iter_utf8(&iter, NULL));
	    }
	    else if(strcmp(key, "realTimeEndpoint") == 0) {
		ci->setHttpEndpoint(bson_iter_utf8(&iter, NULL));
	    }
	}
    }

    if(ci->isValid())
	return ci;
    else
	return nullptr;
}
