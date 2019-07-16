
#include "testspooler.h"

TestSpooler::TestSpooler() : MongoSpooler(nullptr) {
    
}

void TestSpooler::enqueue(std::vector<Record*> &recs) {
    for (auto *rec : recs) {
	recordQueue.push_back(rec);
    }
}
