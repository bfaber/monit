#include <stdexcept>

#include "record.h"
#include "util.h"

// This vector of MatchBundles must all be of the same collection or exception is thrown
Record::Record(std::vector<MatchBundle*> &mbs) {
    addRecords(mbs);
}

void Record::addRecords(std::vector<MatchBundle*> &mbs) {
    for(auto *mb : mbs) {
	
	if(collectionName.empty()) {
	    collectionName = mb->getConfig()->getCollectionName();
	} else if(collectionName != mb->getConfig()->getCollectionName()) {
	    throw std::invalid_argument("collection of MatchBundle doesn't match Record");
	}

	// build the bson doc
	// the doc captures the values from the matchbundle
	ConfigItem *config = mb->getConfig();
	for(auto group : mb->getBundle()) {
	    bson_t *doc = zipUpCsvWithGroups(config->getCsv(), group);
	    docs.push_back(doc);			     
	}
    }
}

bson_t* Record::zipUpCsvWithGroups(std::string csv, std::vector<std::string> &group) {
    auto csvVec = Util::splitString(csv, ',');
    std::vector<std::pair<std::string, std::string>> pairsList;
    bson_t *doc = bson_new();
    if(csvVec.size() == group.size()) {
	// TODO: implement our own zip with
	for(int i = 0; i < csvVec.size(); i++) {
	    bson_append_utf8(doc,
			     csvVec[i].c_str(),
			     csvVec[i].size(),
			     group[i].c_str(),
			     group[i].size());

	}
    }
    return doc;
}

const bson_t** Record::getDocs() {
    /*    
    for(int i = 0; i < docs.size(); i++) {
	docsArray[i] = docs[i];
    }
	
    return docsArray;
    */
    return const_cast<const bson_t**>(&docs[0]);
}

std::string Record::getCollectionName() {
    return collectionName;
}

size_t Record::size() {
    return docs.size();
}

