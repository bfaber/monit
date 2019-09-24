#ifndef MONGOSPOOLER_H
#define MONGOSPOOLER_H

#include <string>
#include <vector>

#include "matchbundle.h"
#include "mongointerface.h"
/**
 *  Encapsulate Mongo instance data, and interface to mongo
 *   This is held by csvprocessor which isnt doing too much rn, will
 *    be fine to have that block while this commits.
 *    Main concern is not blocking reading of log files. 
 */
class MongoSpooler {
public:
    //MongoSpooler(MongoInterface *mi);
    MongoSpooler(std::unique_ptr<MongoInterface> mi);
    MongoSpooler(MongoSpooler&& from);
    virtual void enqueue(std::vector<Record*> &recs);

    int commitToMongo();
    
private:
    std::unique_ptr<MongoInterface> interface;
    std::vector<Record*> recordQueue;    

};
#endif
