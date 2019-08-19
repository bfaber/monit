#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include <string>
#include <mongoc/mongoc.h>

#include "matchbundle.h"

//TODO: eventually subclass this with MongoRecord, etc.
// Record class encapsulates all records going to same collection
class Record {
 public:
    Record(std::vector<MatchBundle*> &mbs);
    // TODO: ~Record { for doc : docs: bson_destroy(doc), mongoc_collection_destroy(collection) }
    void addRecords(std::vector<MatchBundle*> &mbs);
    const bson_t** getDocs();
    size_t size();
    std::string getCollectionName();
    mongoc_collection_t* getCollection();
    void setCollection(mongoc_collection_t* coll);
private:
    std::string collectionName;
    std::vector<bson_t*> docs;
    bson_t* zipUpCsvWithGroups(std::string csv, std::vector<std::string> &group);
    mongoc_collection_t* collection; // initialized on insertion
};
#endif
