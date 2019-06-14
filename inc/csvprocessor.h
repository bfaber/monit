#ifndef CSVPROCESSOR_H
#define CSVPROCESSOR_H

#include <vector>

class CSVProcessor {

 public:
    CSVProcessor(const char* csv);
    ~CSVProcessor();

    void receiveMatches(std::vector<const char*> *matches);
    std::vector<const char*>* getMatches();
    
private:
    const char* csv;
    std::vector<const char*> *matches;
};
#endif
