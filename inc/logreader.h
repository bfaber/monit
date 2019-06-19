#ifndef LOGREADER_H
#define LOGREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

#include "pcre.h"
#include "csvprocessor.h"

class LogReader {
    friend class LogReaderTest;
public:
    LogReader(const char* filename, const char* regex, MatchProcessor *p);
    LogReader(LogReader&& lr);
    LogReader(LogReader const &lr);
    ~LogReader();

    void operator()();
    static std::thread start(LogReader *inst);
    void shutdownThread();

private:

    std::mutex mutex;
    bool shutdown;
    
    const char* logfilename;
    std::ifstream logfile;

    MatchProcessor* processor;
    const char* regex;
    const pcre* compiled_regex;

    bool threadStarted;
    bool threadStopped;


    void startThread();
    void beginLoop();
    bool readFile();
};
#endif
