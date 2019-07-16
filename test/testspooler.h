#ifndef TESTSPOOLER_H
#define TESTSPOOLER_H

#include <vector>

#include "mongospooler.h"
#include "record.h"


class TestSpooler : public MongoSpooler {

 public:
    TestSpooler();

    virtual void enqueue(std::vector<Record*> &recs);

    std::vector<Record*> recordQueue;

};
#endif
