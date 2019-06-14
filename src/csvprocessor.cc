#include "csvprocessor.h"

CSVProcessor::CSVProcessor(const char* csv) : csv(strdup(csv)) {

}

CSVProcessor::~CSVProcessor() {
    delete csv;
}

void CSVProcessor::receiveMatches(std::vector<const char*> *matches)  {

    printf("len of matches %d\n", matches->size());
    this->matches = matches;
}

std::vector<const char*>* CSVProcessor::getMatches() {
    return matches;
}
