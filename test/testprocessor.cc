#include "testprocessor.h"

void TestProcessor::receiveMatches(MatchBundle *ms) {
    this->matches = ms;    
}

MatchBundle* TestProcessor::getMatches() {
    return this->matches;
}
