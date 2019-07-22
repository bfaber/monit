#ifndef MONGORECORDPROCESSOR_H
#define MONGORECORDPROCESSOR_H

#include <vector>
#include <string>
#include <map>
#include <mutex>

#include "recordprocessorinterface.h"
#include "matchbundle.h"
#include "mongospooler.h"
#include "matchbuffer.h"

/**
 *  Runs in a RecordExecutor, pulls data from the LogReader
 *   and formats for Mongo.
 *
 */
class MongoRecordProcessor : public RecordProcessorInterface {
 public:
    MongoRecordProcessor(MongoSpooler *ms);
        
    virtual void receiveMatches(MatchBundle *matches) override;
    void processMatches();

 private:
    std::mutex mutex;
    MatchBuffer matchBuffer;
    MongoSpooler *spooler;

};
#endif
