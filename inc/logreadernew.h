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
    int  findGroups(const char* text, const pcre *comiledRegex, pcre_jit_stack* jit, pcre_extra *extra, std::vector<std::string> &groups);
    const pcre* compileRegex(std::string regex);


    // will batch up the configItem data per filename so that we're only opening
    // a single file per set of regex

    std::map<std::string, std::vector<MatchBundle*>> matchBundlesPerFilename;
    
    RecordProcessorInterface* processor;
};
#endif
