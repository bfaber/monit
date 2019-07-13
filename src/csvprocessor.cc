#include <algorithm>
#include <string>
#include <map>
#include <mongoc/mongoc.h>

#include "csvprocessor.h"
#include "util.h"
#include "record.h"

// TODO: fix ownership of csv
CSVProcessor::CSVProcessor(MongoSpooler *mongoSpooler) : spooler(mongoSpooler),		    
							 shutdown(false)  {

}

CSVProcessor::~CSVProcessor() {
    delete spooler;
}

CSVProcessor::CSVProcessor(CSVProcessor&& other) : spooler(std::move(other.spooler)),
						   matchBuffer(other.matchBuffer),
						   shutdown(other.shutdown) {
}

CSVProcessor::CSVProcessor(CSVProcessor const &other) : spooler(other.spooler),
							matchBuffer(other.matchBuffer),
							shutdown(other.shutdown) {
}

void CSVProcessor::operator() () {
    processMatches();
}

std::thread CSVProcessor::start(CSVProcessor *inst) {
    std::thread th(*inst);
    return th;
}

void CSVProcessor::shutdownThread() {
    // bail us out of the processMatches thread/while loop
    mutex.lock();
    shutdown = true;
    mutex.unlock();
}

// TODO: at some point it will be more of a bundle of objects that
// have been scraped and will be batched up with approps csv.
void CSVProcessor::receiveMatches(MatchBundle *mb) {
    mutex.lock();
    matchBuffer.push_back(mb);
    mutex.unlock();
}

void CSVProcessor::processMatches() {
    printf("Processing Matches from new CSVThread!\n");
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
    for(auto *mb : matchBuffer) {
	bundlesByName[mb->getConfig()->getCollectionName()].push_back(mb);
    }
    matchBuffer.clear();
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

size_t CSVProcessor::getMatchBufferSize() {
    return matchBuffer.size();
}
