#ifndef LOGREADEREXECUTOR_H
#define LOGREADEREXECUTOR_H

#include <thread>
#include <mutex>

#include "logreadernew.h"

class LogReaderExecutor {

 public:
    LogReaderExecutor(std::unique_ptr<LogReaderNew> lr) : logreader(std::move(lr)),
						      stopThread(false),
						      logreaderStarted(false),
						      logreaderStopped(false) {};
    /*
    LogReaderExecutor(const LogReaderExecutor &other) : logreader(std::move(other.logreader)),
							stopThread(other.stopThread),
							logreaderStarted(other.logreaderStarted),
							logreaderStopped(other.logreaderStopped) {};
    */
    LogReaderExecutor(LogReaderExecutor&& other) : logreader(std::move(other.logreader)),
						   stopThread(other.stopThread),
						   logreaderStarted(other.logreaderStarted),
						   logreaderStopped(other.logreaderStopped) {};
    void operator()();
    static std::thread start(LogReaderExecutor *inst);
    void stop();

private:
    std::mutex mutex;
    std::unique_ptr<LogReaderNew> logreader;
    bool logreaderStarted;
    bool logreaderStopped;
    bool stopThread;
};
#endif
