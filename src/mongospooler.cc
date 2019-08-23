#include "mongospooler.h"

MongoSpooler::MongoSpooler(MongoInterface *mi) : interface(mi) {
}

void MongoSpooler::enqueue(std::vector<Record*> &recs) {
    for(auto *rec : recs) {
	recordQueue.push_back(rec);
    }
}

// take the collection name, the pairs, and insert into mongo.
int MongoSpooler::commitToMongo() {
    printf("CommitToMongo %lu records\n", recordQueue.size());
    int result = interface->insertRecords(recordQueue);
    printf("commit to mongo result: %d\n", result);
    // clear recordQueue
    for( auto *rec : recordQueue ) {
	delete rec;
    }
    recordQueue.clear();
    
	/*
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    const char *collection_name = "test";
    bson_t query;
    char *str;
    const char *uri_string = "mongodb://127.0.0.1/?appname=client-example";
    mongoc_uri_t *uri;

    mongoc_init ();

    uri = mongoc_uri_new_with_error (uri_string, &error);
    if (!uri) {
	fprintf (stderr,
		 "failed to parse URI: %s\n"
		 "error message:       %s\n",
		 uri_string,
		 error.message);
	return EXIT_FAILURE;
    }

    client = mongoc_client_new_from_uri (uri);
    if (!client) {
	return EXIT_FAILURE;
    }

    mongoc_client_set_error_api (client, 2);

    bson_init (&query);

    collection = mongoc_client_get_collection (client, "test", collection_name);
    cursor = mongoc_collection_find_with_opts (
					       collection,
					       &query,
					       NULL,  // additional options
					       NULL); // read prefs, NULL for default

    while (mongoc_cursor_next (cursor, &doc)) {
	str = bson_as_canonical_extended_json (doc, NULL);
	fprintf (stdout, "%s\n", str);
	bson_free (str);
    }

    if (mongoc_cursor_error (cursor, &error)) {
	fprintf (stderr, "Cursor Failure: %s\n", error.message);
	return EXIT_FAILURE;
    }

    bson_destroy (&query);
    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);
    mongoc_uri_destroy (uri);
    mongoc_client_destroy (client);
    mongoc_cleanup ();
*/
    return EXIT_SUCCESS;

}
