#ifndef MONGOINTERFACE_H
#define MONGOINTERFACE_H

#include <vector>
#include <string>
#include <mongoc/mongoc.h>
#include "configitem.h"
#include "record.h"

class MongoInterface {
public:
    MongoInterface(std::string host, int port, std::string dbName);
    ~MongoInterface();
    std::vector<ConfigItem*>* getConfigs(std::string collectionName);
    virtual int insertRecords(std::vector<Record*> &recs);
    
private:
    ConfigItem* parseConfigFromBson(const bson_t *doc);

    std::vector<ConfigItem> configs;
    std::string mongoUri;
    std::string dbName;
};
#endif
