#include "testprocessor.h"

void TestProcessor::receiveMatches(std::vector<const char*> *ms) {
    this->matches = ms;    
}

std::vector<const char*>* TestProcessor::getMatches() {
    return this->matches;
}
