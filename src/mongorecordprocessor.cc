
#include "mongorecordprocessor.h"

MongoRecordProcessor::MongoRecordProcessor(std::unique_ptr<MongoSpooler> ms) : spooler(std::move(ms)) {
    //mutex = new std::mutex();
}

MongoRecordProcessor::MongoRecordProcessor(MongoRecordProcessor&& from) : spooler(std::move(from.spooler)) {
    // leave empty for now, only moved on startup before anything created

}
// sharing ownership of these matchbundles with the logreader.
// long lived and the matchbuffer will take the matches and buffer
// them up on those bundles.
// This happens on startup, before threads are started...
void MongoRecordProcessor::addMatchHandler(MatchBundle *mb) {
    matchBuffer.addHandler(mb);
}

size_t MongoRecordProcessor::getMatchBufferSize() {
    return matchBuffer.size();
}

// NO filereading is happening while this is called.
// processor thread might be running, hence mutex on recBuffer
// This is happening in context of the filereader thread.
void MongoRecordProcessor::receiveMatches() {
    
    //printf("Flush match buffers; size: %lu\n", matchBuffer.size());

    auto &bundlesByName = matchBuffer.getMatchesByName();

    std::map<std::string, Record*> recordsByName;
    for(auto& kv : bundlesByName) {
	recordsByName[kv.first] = new Record(kv.second);
    }

    // This section can be lockless or use more fine grain locking since recBuffer is
    // just a list of records.
    // maybe even better if it was a linked list of records, could hand that list out to
    // the spooler without doing any copying...
    mutex.lock();
    for(auto& kv : recordsByName) {
	// here we check if rec is valid...
	// this is really a perf consideration since we're iterating here anyway.
	// because bundles get shuffled when enqueued, we just ensure that
	// no empty Record objects make it to the spooler here.
	// This instead of iterating through them before enqueuing them here.
	if(kv.second->size() > 0) {
	    recBuffer.push_back(kv.second);
	} else {
	    delete kv.second;
	}
	
    }

    mutex.unlock();
    matchBuffer.clearBuffers();
}

// This is happening in context of the recordprocessor thread
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
    // let executor sleep a bit if nothing is here.
    if(recBuffer.size() > 0) {
	mutex.lock();
	spooler->enqueue(recBuffer);
	recBuffer.clear();
	mutex.unlock();
	return true;
    } else {
	return false;
    }
}


