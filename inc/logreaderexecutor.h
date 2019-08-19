#ifndef LOGREADEREXECUTOR_H
#define LOGREADEREXECUTOR_H

#include <thread>
#include <mutex>

#include "logreadernew.h"

class LogReaderExecutor {

 public:
    LogReaderExecutor(LogReaderNew *lr) : logreader(lr),
					  stopThread(false),
					  logreaderStarted(false),
					  logreaderStopped(false) {};

    LogReaderExecutor(LogReaderExecutor const &other) : logreader(other.logreader),
							stopThread(other.stopThread),
							logreaderStarted(other.logreaderStarted),
							logreaderStopped(other.logreaderStopped) {};
    
    LogReaderExecutor(LogReaderExecutor&& other) : logreader(other.logreader),
						   stopThread(other.stopThread),
						   logreaderStarted(other.logreaderStarted),
						   logreaderStopped(other.logreaderStopped) {};
    void operator()();
    static std::thread start(LogReaderExecutor *inst);
    void stop();

private:
    std::mutex mutex;
    LogReaderNew *logreader;
    bool logreaderStarted;
    bool logreaderStopped;
    bool stopThread;
};
#endif
