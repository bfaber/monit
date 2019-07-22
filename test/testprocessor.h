#ifndef TESTPROCESSOR_H
#define TESTPROCESSOR_H

#include <vector>
#include "recordprocessorinterface.h"
#include "matchprocessor.h"
#include "matchbundle.h"
#include "mongospooler.h"
#include "matchbuffer.h"

class TestProcessor : public RecordProcessorInterface {
public:
    TestProcessor(MongoSpooler *ms);
		
    virtual void receiveMatches(MatchBundle *matches) override;
    void processMatches();

    void getMatches(std::vector<MatchBundle*> &ms);
    
private:
    const char* csv;
    std::vector<MatchBundle*> testmatches;
    MongoSpooler *spooler;
};
#endif
