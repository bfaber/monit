#include "testmongointerface.h"

int TestMongoInterface::insertRecords(std::vector<Record*> &recs) {
    for (auto *rec : recs) {
	recordQueue.push_back(rec);
    }
    return 0;
}
