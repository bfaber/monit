#ifndef LOGREADER_H
#define LOGREADER_H

#include <iostream>
#include <fstream>
#include <vector>

#include "pcre.h"
#include "csvprocessor.h"

class LogReader {

public:
    LogReader(const char* filename, const char* regex, CSVProcessor *p);
    LogReader(LogReader&& lr);
    LogReader(LogReader const &lr);
    ~LogReader();

    void operator()();
    static void start(LogReader *inst);
    
private:
    
    const char* logfilename;
    std::ifstream logfile;

    CSVProcessor* processor;
    const char* regex;
    const pcre* compiled_regex;

    bool threadStarted;
    bool threadStopped;


    void startThread();
    void beginLoop();
    bool readFile();
};
#endif
