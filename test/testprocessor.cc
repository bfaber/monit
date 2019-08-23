#include "testprocessor.h"

TestProcessor::TestProcessor(MongoSpooler *ts) : spooler(ts) {}

void TestProcessor::addMatchHandler(MatchBundle *mb) {
    testmatches.push_back(mb);
}

size_t TestProcessor::size() {
    return testmatches.size();
}

void TestProcessor::receiveMatches() {
}

void TestProcessor::getMatches(std::vector<MatchBundle*> &ms) {
    for(auto *mb : testmatches) {
	printf("returning a match from test processor!\n");
	ms.push_back(mb);
    }
}

bool TestProcessor::processMatches() {

}
