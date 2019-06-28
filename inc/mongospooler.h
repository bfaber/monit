#ifndef MONGOSPOOLER_H
#define MONGOSPOOLER_H

#include <string>
#include <vector>
#include <mutex>

#include "mongointerface.h"
/**
 *  Encapsulate Mongo instance data, and interface to mongo
 *  Runs in own thread, takes all enqueued inserts and batch commits them.
 */
class MongoSpooler {
public:
    MongoSpooler(MongoInterface *mi);    
    void enqueue(std::vector<std::pair<std::string, std::string>> matches);

private:
    MongoInterface *interface;
    std::mutex mutex;
    std::vector<std::pair<std::string, std::string>> matchQueue;
    
    int commitToMongo(std::string query);
};
#endif
