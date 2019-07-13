#ifndef MATCHBUNDLE_H
#define MATCHBUNDLE_H

#include <vector>
#include <string>
#include "configitem.h"

/**
 *  Encapsulate the group/csv pairs, as well as any ancillary data 
 *  associated with them.
 *
 *  TODO: inherit this from configitem, and just cast to matchbundle
 *  on setting of capturedGroups?
 */
class MatchBundle {
public:
    MatchBundle(ConfigItem *ci);

    void addGroups(std::vector<std::string> groups);
    size_t size();
    ConfigItem* getConfig();
    std::vector<std::vector<std::string>> getBundle();
    
private:
    ConfigItem *config;
    std::vector<std::vector<std::string>> groupBundle;

};
#endif
