#ifndef TESTPROCESSOR_H
#define TESTPROCESSOR_H

#include <vector>
#include "matchprocessor.h"
#include "matchbundle.h"

class TestProcessor : public MatchProcessor {
public:
    TestProcessor(const char* csv) : csv(strdup(csv)) {};
		

    virtual void receiveMatches(MatchBundle *matches) override;
    virtual MatchBundle* getMatches() override;

private:
    const char* csv;
    MatchBundle *matches;
};
#endif
