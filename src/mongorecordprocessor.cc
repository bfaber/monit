
#include "mongorecordprocessor.h"

MongoRecordProcessor::MongoRecordProcessor(MongoSpooler *ms) : spooler(ms) {
}


// TODO: at some point it will be more of a bundle of objects that
// have been scraped and will be batched up with approps csv.
void MongoRecordProcessor::receiveMatches(MatchBundle *mb) {
    mutex.lock();
    printf("pushing back a match on MongoRecordProcessor!\n");
    matchBuffer.addMatches(mb);
    mutex.unlock();
}

void MongoRecordProcessor::processMatches() {

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
	recs.push_back(kv.second);
    }

    spooler->enqueue(recs);
}


