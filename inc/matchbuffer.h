#ifndef MATCHBUFFER_H
#define MATCHBUFFER_H

#include <map>
#include <vector>
#include <string>

#include "matchbundle.h"

class MatchBuffer {
 public:
    
    void addMatches(MatchBundle *matches);
    void getMatchesByName(std::map<std::string, std::vector<MatchBundle*>> &matchesByName);
    void clearBuffers();
    
 private:
    std::map<std::string, std::vector<MatchBundle*>> matchesByName;

};
#endif
