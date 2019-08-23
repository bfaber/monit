#ifndef RECORDPROCESSORINTERFACE_H
#define RECORDPROCESSORINTERFACE_H

#include "matchbundle.h"

/**
 *  Defines an interface for the LogReader to send matches to.
 *  
 *
 */
class RecordProcessorInterface {
 public:
    virtual void addMatchHandler(MatchBundle *mb) = 0;
    virtual void receiveMatches() = 0;
    virtual bool processMatches() = 0;
};
#endif
