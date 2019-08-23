#include "matchbundle.h"

MatchBundle::MatchBundle(ConfigItem *ci) : config(ci) {}
MatchBundle::~MatchBundle() {
    // dont del config.    
}

MatchBundle::MatchBundle(MatchBundle *mb) {
    config = mb->config;
    for( auto groups : mb->groupBundle ) {
	groupBundle.push_back(groups);
    }
}

void MatchBundle::addGroups(std::vector<std::string> groups) {
    groupBundle.push_back(groups);
}

size_t MatchBundle::size() {
    return groupBundle.size();
}

ConfigItem* MatchBundle::getConfigItem() {
    return config;
}

std::vector<std::vector<std::string>> MatchBundle::getBundle() {
    return groupBundle;
}

void MatchBundle::clear() {
    for( auto groups : groupBundle ) {
	groups.clear();
    }
    groupBundle.clear();
}
