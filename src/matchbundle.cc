#include "matchbundle.h"

MatchBundle::MatchBundle(ConfigItem *ci) : config(ci),
					   retryTime(0) {
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

void MatchBundle::retry(long atTimeMs) {
    retryTime = atTimeMs;
}

long MatchBundle::getRetryTime() {
    return retryTime;
}
