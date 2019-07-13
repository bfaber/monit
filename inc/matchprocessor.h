#ifndef MATCH_PROCESSOR_H
#define MATCH_PROCESSOR_H

#include <vector>
#include "matchbundle.h"

class MatchProcessor {

public:
    virtual void receiveMatches(MatchBundle *matches) = 0;
    virtual size_t getMatchBufferSize() = 0;
    
};
#endif

