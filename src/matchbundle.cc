#include "matchbundle.h"

MatchBundle::MatchBundle(ConfigItem *ci) : config(ci) {
}

void MatchBundle::addGroups(std::vector<std::string> groups) {
    groupBundle.push_back(groups);
}

size_t MatchBundle::size() {
    return groupBundle.size();
}

ConfigItem* MatchBundle::getConfig() {
    return config;
}

std::vector<std::vector<std::string>> MatchBundle::getBundle() {
    return groupBundle;
}
