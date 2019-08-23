#ifndef PROCESSOREXECUTOR_H
#define PROCESSOREXECUTOR_H

#include <thread>
#include <mutex>

#include "mongorecordprocessor.h"
#include "mongospooler.h"

class ProcessorExecutor {
 public:
    
     ProcessorExecutor(MongoRecordProcessor *rp, MongoSpooler *ms) : recordProcessor(rp),
	mongoSpooler(ms),
	stopThread(false),	
	processorStarted(false),
	processorStopped(false) {};
    
    ProcessorExecutor(ProcessorExecutor const &other) :
	mongoSpooler(other.mongoSpooler),
	stopThread(other.stopThread),
	processorStarted(other.processorStarted),
	processorStopped(other.processorStopped) { recordProcessor = other.recordProcessor; printf("copy ctor &other\n");};
    
    ProcessorExecutor(ProcessorExecutor&& other) : recordProcessor(other.recordProcessor),
	mongoSpooler(other.mongoSpooler),
	stopThread(other.stopThread),
	processorStarted(other.processorStarted),
						   processorStopped(other.processorStopped) { printf("move ctor &&other\n");};
    
    void operator()();
    static std::thread start(ProcessorExecutor *inst);
    void stop();

private:
    std::mutex mutex;
    MongoRecordProcessor *recordProcessor;
    MongoSpooler *mongoSpooler;
    bool processorStarted;
    bool processorStopped;
    bool stopThread;
};
#endif
