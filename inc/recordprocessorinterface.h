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
    virtual void receiveMatches(MatchBundle *matches) = 0;
};
#endif
