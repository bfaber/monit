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
#include "filebundle.h"

class LogReaderNew {
public:
    LogReaderNew(std::vector<ConfigItem*>* configs, RecordProcessorInterface *p);

    bool readFiles();
    
private:
    // will batch up the configItem data per filename so that we're only opening
    // a single file per set of regex

    std::map<std::string, FileBundle*> matchBundlesPerFilename;
    
    RecordProcessorInterface* processor;
};
#endif
