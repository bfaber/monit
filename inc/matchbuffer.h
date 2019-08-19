#ifndef MATCHBUFFER_H
#define MATCHBUFFER_H

#include <map>
#include <vector>
#include <string>

#include "matchbundle.h"

class MatchBuffer {
 public:
    MatchBuffer();
    void addMatches(MatchBundle *matches);
    void getMatchesByName(std::map<std::string, std::vector<MatchBundle*>> &matchesByName);
    void clearBuffers();
    size_t size();
    
 private:
    std::map<std::string, std::vector<MatchBundle*>> matchesByName;
    int count;
};
#endif
