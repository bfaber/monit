
#include "testspooler.h"

TestSpooler::TestSpooler() : MongoSpooler(nullptr) {
    
}

void TestSpooler::enqueue(std::vector<Record*> &recs) {
    for (auto *rec : recs) {
	recordQueueTest.push_back(rec);
    }
}

Record* TestSpooler::getRecord(int index) {
    if(recordQueueTest.size() < index)
	return nullptr;
    else
	return recordQueueTest[index];
}

size_t TestSpooler::size() {
    return recordQueueTest.size();
}
