#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include <string>
#include <mongoc/mongoc.h>

#include "matchbundle.h"

class Record {
 public:
    Record(std::vector<MatchBundle*> &mbs);
    void addRecords(std::vector<MatchBundle*> &mbs);
    
private:
    std::string collectionName;
    std::vector<bson_t*> docs;
    bson_t* zipUpCsvWithGroups(std::string csv, std::vector<std::string> &group);
};
#endif
