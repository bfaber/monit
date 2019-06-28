
#include "mongointerface.h"

#include <sstream>

MongoInterface::MongoInterface(std::string host, int port, std::string dbName) : dbName(dbName) {

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

void MongoInterface::insertRecords(std::string query) {

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
