#ifndef PROCESSOREXECUTOR_H
#define PROCESSOREXECUTOR_H

#include <thread>
#include <mutex>

#include "mongorecordprocessor.h"
#include "mongospooler.h"

class ProcessorExecutor {
 public:
    
    ProcessorExecutor(std::shared_ptr<MongoRecordProcessor> rp) : recordProcessor(rp),	       
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
						   stopThread(other.stopThread),
						   processorStarted(other.processorStarted),
						   processorStopped(other.processorStopped) {};
    
    void operator()();
    static std::thread start(ProcessorExecutor *inst);
    void stop();

private:
    std::mutex mutex;
    std::shared_ptr<MongoRecordProcessor> recordProcessor;
    bool processorStarted;
    bool processorStopped;
    bool stopThread;
};
#endif
