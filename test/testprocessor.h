#ifndef TESTPROCESSOR_H
#define TESTPROCESSOR_H

#include <vector>
#include "matchprocessor.h"

class TestProcessor : public MatchProcessor {
public:
    TestProcessor(const char* csv) : csv(strdup(csv)) {};

    virtual void receiveMatches(std::vector<const char*> *matches) override;
    virtual std::vector<const char*>* getMatches() override;

private:
    const char* csv;
    std::vector<const char*> *matches;
};
#endif
