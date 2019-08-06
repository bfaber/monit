#ifndef TESTMONGOINTERFACE_H
#define TESTMONGOINTERFACE_H

#include <vector>

#include "record.h"
#include "mongointerface.h"

class TestMongoInterface : public MongoInterface {
 public:
    virtual int insertRecords(std::vector<Record*> &recs) override;
    std::vector<Record*> recordQueue;
    
};
#endif
