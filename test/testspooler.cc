
#include "testspooler.h"

TestSpooler::TestSpooler() : MongoSpooler(nullptr) {
    
}

void TestSpooler::enqueue(std::vector<Record*> &recs) {
    for (auto *rec : recs) {
	recordQueueTest.push_back(rec);
    }
}

Record* TestSpooler::popRecord() {
    if(recordQueueTest.size() > 0)
	return recordQueueTest[0];
    else
	return nullptr;
}
