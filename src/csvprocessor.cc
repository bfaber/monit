#include "csvprocessor.h"
#include "util.h"
#include <algorithm>
#include <string>

// TODO: fix ownership of csv
CSVProcessor::CSVProcessor(const char* csv) : csv(strdup(csv)),
					      shutdown(false)  {
    matches = new std::vector<const char*>();
}

CSVProcessor::~CSVProcessor() {
    delete csv;
    csv = nullptr;
}

CSVProcessor::CSVProcessor(CSVProcessor&& other) : csv(strdup(other.csv)),
						   shutdown(other.shutdown) {
    matches = new std::vector<const char*>();
    for (auto *m : *other.matches)
	matches->push_back(m);		  
}

CSVProcessor::CSVProcessor(CSVProcessor const &other) : csv(strdup(other.csv)),
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

    std::string csvStr(csv);

    auto csvs = Util::splitString(csvStr, ',');
    
    
}

std::vector<const char*>* CSVProcessor::getMatches() {
    printf("getmatches %d\n", matches->size());
    return matches;
}
