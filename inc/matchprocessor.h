#ifndef MATCH_PROCESSOR_H
#define MATCH_PROCESSOR_H

#include <vector>

class MatchProcessor {

public:
    virtual void receiveMatches(std::vector<const char*> *matches) = 0;
    virtual std::vector<const char*>* getMatches() = 0;
    
};
#endif

