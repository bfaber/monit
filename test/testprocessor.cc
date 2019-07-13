#include "testprocessor.h"

void TestProcessor::receiveMatches(MatchBundle *ms) {
    this->matches = ms;    
}

size_t TestProcessor::getMatchBufferSize() {
    return this->matches->size();
}
