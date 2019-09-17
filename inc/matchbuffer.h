#ifndef MATCHBUFFER_H
#define MATCHBUFFER_H

#include <map>
#include <vector>
#include <string>

#include "matchbundle.h"

class MatchBuffer {
 public:
    void addHandler(MatchBundle *matchbundle);    
    //void getMatchesByName(std::map<std::string, std::vector<MatchBundle*>> &matchesByName);
    std::map<std::string, std::vector<MatchBundle*>>& getMatchesByName();
    void clearBuffers();
    size_t size();
    
 private:
    std::map<std::string, std::vector<MatchBundle*>> matchesByName;

};
#endif
