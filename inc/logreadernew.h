#ifndef LOGREADERNEW_H
#define LOGREADERNEW_H

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <map>

#include "pcre.h"
#include "recordprocessorinterface.h"
#include "configitem.h"

class LogReaderNew {
public:
    LogReaderNew(std::vector<ConfigItem*>* configs, RecordProcessorInterface *p);

    bool readFiles();

    
private:
    const pcre* compileRegex(std::string regex);
    int  findGroups(std::string text, const pcre *comiledRegex, std::vector<std::string> &groups);
    // will batch up the configItem data per filename so that we're only opening
    // a single file per set of regex

    std::map<std::string, std::vector<MatchBundle*>> matchBundlesPerFilename;
    
    RecordProcessorInterface* processor;
};
#endif
