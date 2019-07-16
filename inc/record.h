#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include <string>
#include <mongoc/mongoc.h>

#include "matchbundle.h"

//TODO: eventually subclass this with MongoRecord, etc.
class Record {
 public:
    Record(std::vector<MatchBundle*> &mbs);
    void addRecords(std::vector<MatchBundle*> &mbs);
    const bson_t** getDocs();
    size_t size();
    std::string getCollectionName();
    
    
private:
    std::string collectionName;
    std::vector<bson_t*> docs;
    bson_t* zipUpCsvWithGroups(std::string csv, std::vector<std::string> &group);
};
#endif
