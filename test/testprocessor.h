#ifndef TESTPROCESSOR_H
#define TESTPROCESSOR_H

#include <vector>
#include "matchprocessor.h"
#include "matchbundle.h"

class TestProcessor : public MatchProcessor {
public:
    TestProcessor(const char* csv) : csv(strdup(csv)) {};
		

    virtual void receiveMatches(MatchBundle *matches) override;
    virtual size_t getMatchBufferSize() override;
    MatchBundle* getMatches();
    
private:
    const char* csv;
    MatchBundle *matches;
};
#endif
