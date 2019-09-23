
#include "logreaderexecutor.h"
#include "util.h"

std::thread LogReaderExecutor::start(LogReaderExecutor *inst) {
    std::thread th(*inst);
    return th;
}

void LogReaderExecutor::stop() {
    mutex.lock();
    stopThread = true;
    mutex.unlock();
}

void LogReaderExecutor::operator()() {
    mutex.lock();
    logreaderStarted = true;
    bool stop = stopThread;
    mutex.unlock();

    while(!stop) {
	if( !logreader->readFiles() ) {
	    // TODO: we can get nice here and throttle depending on how much we're reading and how long
	    // it takes to read.
	    // Would be awesome too to have feedback from the db committing and get some
	    // automated optimisation with that.
	    // but maybe it is better to keep the usages here easy to reason about at least for now
	    Util::sleepMs(100);
	}
    }
}
