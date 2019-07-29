#include "testprocessor.h"

TestProcessor::TestProcessor(MongoSpooler *ts) : spooler(ts) {}

void TestProcessor::receiveMatches(std::vector<MatchBundle*> &matches) {
    printf("pushing back match bundle on test processor!\n");
    for(auto *ms : matches) {
	testmatches.push_back(ms);
    }
}

void TestProcessor::getMatches(std::vector<MatchBundle*> &ms) {
    for(auto *mb : testmatches) {
	printf("returning a match from test processor!\n");
	ms.push_back(mb);
    }
}
