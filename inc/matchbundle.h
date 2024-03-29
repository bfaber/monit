#ifndef MATCHBUNDLE_H
#define MATCHBUNDLE_H

#include <vector>
#include <string>
#include "configitem.h"

/**
 *  Encapsulate the group/csv pairs, as well as any ancillary data 
 *  associated with them.
 *
 *  This represents a list of regex matches from a logfile section.
 *   Regex + logfileSection -> vec<groups> (ie MatchBundle)
 *
 */
class MatchBundle {
public:
    MatchBundle(ConfigItem *ci);
    MatchBundle(MatchBundle *mb);
    ~MatchBundle();
    void addGroups(std::vector<std::string> groups);
    size_t size();
    ConfigItem* getConfigItem();
    std::vector<std::vector<std::string>> getBundle();
    void clear();
    
private:
    ConfigItem *config;
    std::vector<std::vector<std::string>> groupBundle;

};
#endif
