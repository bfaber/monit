#include "processorexecutor.h"
#include "util.h"

std::thread ProcessorExecutor::start(ProcessorExecutor *inst) {
    std::thread th(std::move(*inst));
    return th;
}

void ProcessorExecutor::stop() {
    mutex.lock();
    stopThread = true;
    mutex.unlock();
}

void ProcessorExecutor::operator()() {
    mutex.lock();
    processorStarted = true;
    bool stop = stopThread;
    mutex.unlock();
    if( !recordProcessor ) {
	printf("record processor is null!\n");
	//stop();
	stop = true;
    }
    while( !stop ) {
	// this will block as committing to Storage
	if( !recordProcessor->processMatches() ) {
	    Util::sleepMs(1);
	}
    }
}
