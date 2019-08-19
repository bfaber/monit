
#include "mongorecordprocessor.h"

MongoRecordProcessor::MongoRecordProcessor(MongoSpooler *ms) : spooler(ms) {}

size_t MongoRecordProcessor::getMatchBufferSize() {
    return matchBuffer.size();
}

// TODO: at some point it will be more of a bundle of objects that
// have been scraped and will be batched up with approps csv.
void MongoRecordProcessor::receiveMatches(std::vector<MatchBundle*> &bundles) {
    printf("pushing back bundles on MongoRecordProcessor! %lu\n", bundles.size());
    mutex.lock();
    for(auto *mb : bundles) {
	matchBuffer.addMatches(mb);
    }
    mutex.unlock();
}

bool MongoRecordProcessor::processMatches() {

    // take the csv along with the matches, and zip them up as tuples.
    // then send out the vector of tuples to the spooler

    // TODO: the matches should be wrapped in an object that tells
    // us what match was performed and where it is going.
    // That object here would be wrapped with the approp csv
    // and sent to the observer board so that registered endpoints
    // can be notified.
    // For now (6/19) we'll just have a mongo spooler and ship to that.

    // process all queued MatchBundles into a vector<Records>
    // the Record will eventually be subclassed to abstract out the db interface.

    // the Record is whatever the db interface needs to do an insert with min processing.

    // every match going to the same collection should be in the same record.
    // the vector of records should be only of the size of the various collections
    // spec in configitems

    // first thing to do is sort matchbundles by collection, empty the matchBuffer this way
    printf("processmatches matchbuffer size: %lu\n", matchBuffer.size());
    std::map<std::string, std::vector<MatchBundle*>> bundlesByName;
    mutex.lock();
    matchBuffer.getMatchesByName(bundlesByName);
    matchBuffer.clearBuffers();
    mutex.unlock();

    std::map<std::string, Record*> recordsByName;
    for(auto& kv : bundlesByName) {
	recordsByName[kv.first] = new Record(kv.second);
    }

    std::vector<Record*> recs;
    for(auto& kv : recordsByName) {
	// here we check if rec is valid...
	// this is really a perf consideration since we're iterating here anyway.
	// because bundles get shuffled when enqueued, we just ensure that
	// no empty Record objects make it to the spooler here.
	// This instead of iterating through them before enqueuing them here.
	if(kv.second->size() > 0) {
	    recs.push_back(kv.second);
	}
    }
    // let executor sleep a bit if nothing is here.
    if(recs.size() > 0) {
	spooler->enqueue(recs);
	return true;
    } else {
	return false;
    }
}


