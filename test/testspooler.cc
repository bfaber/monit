
#include "testspooler.h"

TestSpooler::TestSpooler() : MongoSpooler(nullptr) {
    
}

void TestSpooler::enqueue(std::vector<Record*> &recs) {
    for (auto *rec : recs) {
	recordQueueTest.push_back(rec);
    }
}

size_t TestSpooler::size() {
    return recordQueueTest.size();
}
