#ifndef MONGOSPOOLER_H
#define MONGOSPOOLER_H

#include <string>
#include <vector>
#include <mutex>

#include "matchbundle.h"
#include "mongointerface.h"
/**
 *  Encapsulate Mongo instance data, and interface to mongo
 *  Runs in own thread, takes all enqueued inserts and batch commits them.
 */
class MongoSpooler {
public:
    MongoSpooler(MongoInterface *mi);

    void enqueue(std::vector<Record*> &recs);

    int commitToMongo();
    
private:
    MongoInterface *interface;
    std::mutex mutex;
    std::vector<Record*> recordQueue;    

};
#endif
