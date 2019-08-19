
#include "logreaderexecutor.h"

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
	logreader->readFiles();
    }
}
