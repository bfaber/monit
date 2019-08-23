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
    virtual void addMatchHandler(MatchBundle *mb) override;
    virtual void receiveMatches() override;
    virtual bool processMatches() override;
    size_t size();
    void getMatches(std::vector<MatchBundle*> &ms);
    
private:
    const char* csv;
    std::vector<MatchBundle*> testmatches;
    MongoSpooler *spooler;
};
#endif
