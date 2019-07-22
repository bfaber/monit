#include "testprocessor.h"

TestProcessor::TestProcessor(MongoSpooler *ts) : spooler(ts) {}

void TestProcessor::receiveMatches(MatchBundle *ms) {
    printf("pushing back match bundle on test processor!\n");
    testmatches.push_back(ms);
}

void TestProcessor::getMatches(std::vector<MatchBundle*> &ms) {
    for(auto *mb : testmatches) {
	printf("returning a match from test processor!\n");
	ms.push_back(mb);
    }
}
