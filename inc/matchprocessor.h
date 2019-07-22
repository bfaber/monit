#ifndef MATCH_PROCESSOR_H
#define MATCH_PROCESSOR_H

#include <vector>
#include "matchbundle.h"

class MatchProcessor {

public:
    void receiveMatches(MatchBundle *matches);
    size_t getMatchBufferSize();
    
};
#endif

