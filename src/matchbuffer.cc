
#include "matchbuffer.h"

MatchBuffer::MatchBuffer() : count(0) {}

void MatchBuffer::addMatches(MatchBundle *matches) {
    //    if(matches->size() > 0) {
	matchesByName[matches->getConfigItem()->getCollectionName()].push_back(matches);
	count++;
	//    }
}

void MatchBuffer::getMatchesByName(std::map<std::string, std::vector<MatchBundle*>> &ref) {
    for(auto &kv : matchesByName) {
	for(auto *mb : kv.second) {
	    ref[kv.first].push_back(mb);
	}
    }
}

void MatchBuffer::clearBuffers() {
    matchesByName.clear();
    count = 0;
}

size_t MatchBuffer::size() {
    return count;
}
