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

    while( !stop ) {
	if( !recordProcessor->processMatches() ) {
	    Util::sleepMs(1);
	} else {
	    mongoSpooler->commitToMongo();
	}
    }
}
