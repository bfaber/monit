#include "testprocessor.h"

TestProcessor::TestProcessor(std::unique_ptr<MongoSpooler> ts) : spooler(std::move(ts)) {}

TestProcessor::TestProcessor(TestProcessor&& from) : spooler(std::move(from.spooler)) {};

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
