
#include "matchbuffer.h"

void MatchBuffer::addMatches(MatchBundle *matches) {
    matchesByName[matches->getConfigItem()->getCollectionName()].push_back(matches);
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
}
