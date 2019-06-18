#ifndef CSVPROCESSOR_H
#define CSVPROCESSOR_H

#include <vector>
#include <thread>
#include <mutex>

#include "matchprocessor.h"

class CSVProcessor : public MatchProcessor {

 public:
    CSVProcessor(const char* csv);
    CSVProcessor(CSVProcessor&& other);
    CSVProcessor(CSVProcessor const &other);
    ~CSVProcessor();
    
    void operator() ();
    static std::thread start(CSVProcessor *inst);
    void shutdownThread();
    
    void receiveMatches(std::vector<const char*> *matches) override;
    std::vector<const char*>* getMatches();
    
private:
    std::thread thread;
    std::mutex mutex;
    bool shutdown;
    
    void processMatches();
    const char* csv;
    std::vector<const char*> *matches;
};
#endif
