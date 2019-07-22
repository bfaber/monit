#ifndef CSVPROCESSOR_H
#define CSVPROCESSOR_H

#include <vector>
#include <string>
#include <thread>
#include <mutex>

#include "matchprocessor.h"
#include "mongospooler.h"
#include "matchbundle.h"

class CSVProcessor : public MatchProcessor {

 public:
    CSVProcessor(MongoSpooler *ms);
    CSVProcessor(CSVProcessor&& other);
    CSVProcessor(CSVProcessor const &other);
    virtual ~CSVProcessor();
    
    void operator() ();
    static std::thread start(CSVProcessor *inst);
    void shutdownThread();
    
    void receiveMatches(MatchBundle *matches) override;
    size_t getMatchBufferSize() override;

private:
    std::thread thread;
    std::mutex mutex;
    bool shutdown;

    void processMatches();    
    std::vector<MatchBundle*> matchBuffer;
    MongoSpooler *spooler;

};
#endif
