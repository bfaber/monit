#ifndef LOGREADER_H
#define LOGREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

#include "pcre.h"
#include "csvprocessor.h"
#include "configitem.h"

class LogReader {
    friend class LogReaderTest;
public:
    LogReader(std::vector<ConfigItem*>* configs, MatchProcessor *p);
    LogReader(LogReader&& lr);
    LogReader(LogReader const &lr);
    ~LogReader();

    void operator()();
    static std::thread start(LogReader *inst);
    void shutdownThread();
    bool isThreadStarted();

    // mostly public for testing reasons. 
    int  findGroups(std::string text, const pcre *comiledRegex, std::vector<std::string> &groups);
    const pcre* compileRegex(std::string regex);

private:

    std::mutex mutex;
    bool shutdown;

    // will batch up the configItem data per filename so that we're only opening
    // a single file per set of regex
    std::vector<ConfigItem*>* configs;

    MatchProcessor* processor;

    bool threadStarted;
    bool threadStopped;


    void startThread();
    void beginLoop();
    bool readFile();

    //    std::map<std::string, ConfigItem*> sortConfigsByFilename(std::vector<ConfigItem*> *configs);
};
#endif
