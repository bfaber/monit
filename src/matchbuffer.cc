
#include "matchbuffer.h"

// This should only have the MatchBundles as generated at the start of the logreader:
// the groups can be copied over into them here.
// size should be basically the number of group vectors bundled in each matchbuffer obj.
// the matchbuffer is in the matchbundle is in the logreader and the mongorecordprocessor

void MatchBuffer::addHandler(MatchBundle *matchHandler) {
    matchesByName[matchHandler->getConfigItem()->getCollectionName()].push_back(matchHandler);
}

std::map<std::string, std::vector<MatchBundle*>>&
MatchBuffer::getMatchesByName() {
    /*    for(auto &kv : matchesByName) {
	for(auto *mb : kv.second) {
	    ref[kv.first].push_back(mb);
	}
	}*/

    return matchesByName;
}

void MatchBuffer::clearBuffers() {
    for( auto &kv : matchesByName ) {
	for( auto *mb : kv.second ) {
	    mb->clear();
	}
    }
}

// how many records have we accrued
size_t MatchBuffer::size() {
    size_t count = 0;
    for( auto &kv : matchesByName ) {
	for( auto *mb : kv.second ) {
	    count += mb->size();
	}
    }
    return count;
}
