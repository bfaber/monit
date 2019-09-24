#ifndef PROCESSOREXECUTOR_H
#define PROCESSOREXECUTOR_H

#include <thread>
#include <mutex>

#include "mongorecordprocessor.h"
#include "mongospooler.h"

class ProcessorExecutor {
 public:
    
     ProcessorExecutor(std::unique_ptr<MongoRecordProcessor> rp,
		       std::unique_ptr<MongoSpooler> ms) : recordProcessor(std::move(rp)),
							   mongoSpooler(std::move(ms)),
							   stopThread(false),	
							   processorStarted(false),
							   processorStopped(false) {};
    /*    
    ProcessorExecutor(ProcessorExecutor const &other) : mongoSpooler(std::move(other.mongoSpooler)),
							stopThread(other.stopThread),
							processorStarted(other.processorStarted),
							processorStopped(other.processorStopped) {
	//recordProcessor = other.recordProcessor; printf("copy ctor &other\n");
    };
    */  
    ProcessorExecutor(ProcessorExecutor&& other) : recordProcessor(std::move(other.recordProcessor)),
						   mongoSpooler(std::move(other.mongoSpooler)),
						   stopThread(other.stopThread),
						   processorStarted(other.processorStarted),
						   processorStopped(other.processorStopped) {};
    
    void operator()();
    static std::thread start(ProcessorExecutor *inst);
    void stop();

private:
    std::mutex mutex;
    std::unique_ptr<MongoRecordProcessor> recordProcessor;
    std::unique_ptr<MongoSpooler> mongoSpooler;
    bool processorStarted;
    bool processorStopped;
    bool stopThread;
};
#endif
