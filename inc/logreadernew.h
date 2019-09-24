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

    LogReaderNew(std::vector<ConfigItem*>* configs, std::unique_ptr<RecordProcessorInterface> p);
    LogReaderNew(LogReaderNew&& from);

    bool readFiles();
    
private:
    void init(std::vector<ConfigItem*> *configs);
    // will batch up the configItem data per filename so that we're only opening
    // a single file per set of regex

    std::map<std::string, FileBundle*> matchBundlesPerFilename;
    
    std::unique_ptr<RecordProcessorInterface> processor;
};
#endif
