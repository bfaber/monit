#include "csvprocessor.h"
#include <algorithm>

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
    
void CSVProcessor::receiveMatches(std::vector<const char*> *matches) {

    printf("len of matches %d\n", matches->size());
    this->matches = matches;
        
}

void CSVProcessor::processMatches() {
    printf("Processing Matches from new CSVThread!\n");
}

std::vector<const char*>* CSVProcessor::getMatches() {
    return matches;
}
