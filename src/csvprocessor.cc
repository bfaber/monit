#include "csvprocessor.h"
#include "util.h"
#include <algorithm>
#include <string>
#include <mongoc/mongoc.h>

// TODO: fix ownership of csv
CSVProcessor::CSVProcessor(MongoSpooler *mongoSpooler) : spooler(mongoSpooler),		    
							 shutdown(false)  {
    matches = new std::vector<const char*>();
}

CSVProcessor::~CSVProcessor() {
    delete spooler;
}

CSVProcessor::CSVProcessor(CSVProcessor&& other) : spooler(std::move(other.spooler)),
						   shutdown(other.shutdown) {
    matches = new std::vector<const char*>();
    for (auto *m : *other.matches)
	matches->push_back(m);		  
}

CSVProcessor::CSVProcessor(CSVProcessor const &other) : spooler(other.spooler),
							shutdown(other.shutdown) {
    matches = new std::vector<const char*>();
    for (auto *m : *other.matches)
	matches->push_back(m);
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
void CSVProcessor::receiveMatches(std::vector<const char*> *matches) {
    
    printf("len of matches %d\n", matches->size());
    this->matches = matches;
}

void CSVProcessor::processMatches() {
    printf("Processing Matches from new CSVThread!\n");
    // take the csv along with the matches, and zip them up as tuples.
    // then send out the vector of tuples to the spooler

    // TODO: the matches should be wrapped in an object that tells
    // us what match was performed and where it is going.
    // That object here would be wrapped with the approp csv
    // and sent to the obeserver board so that registered endpoints
    // can be notified.
    // For now (6/19) we'll just have a mongo spooler and ship to that.

    std::vector<std::pair<std::string, std::string>> pairs;    
    const char* csv = "some,csv";
    std::string csvStr(csv);
    auto csvs = Util::splitString(csvStr, ',');

    // build column value pairs
    if(csvs.size() == matches->size()) {
	for (int i = 0; i < csvs.size(); i++) {
	    std::string p1(csvs[i]);
	    const char* p2char = matches->at(i); // throws exception if out of bounds
	    std::string p2(p2char);
	    std::pair<std::string, std::string> pair(p1, p2);
	    pairs.push_back(pair);
	}
    }
    
    // now build up transaction with mongo from pairs
    spooler->enqueue(pairs);    
}

std::vector<const char*>* CSVProcessor::getMatches() {
    printf("getmatches %d\n", matches->size());
    return matches;
}
