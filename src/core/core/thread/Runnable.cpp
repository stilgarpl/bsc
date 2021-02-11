#include <utility>
#include <exception>

//
// Created by 23stilgar on 20.08.17.
//

#include "Runnable.h"
using namespace std::chrono_literals;

void bsc::Runnable::start() {
    std::lock_guard g(startMutex);
    if (!threadStarted) {
        threadStarted = true;
        thread        = ContextRunner::runNewThread(std::ref(*this));
    }
    //@todo throw exception on already started thread?
}

void bsc::Runnable::operator()() {
    onStart();
    started = true;
    run();
    onStop();
    finished = true;
}

bsc::Runnable::~Runnable() {
    if (!finished) {
        stop();
    }
    join();
}

void bsc::Runnable::stop() {
    std::unique_lock<std::mutex> g(stopMutex);
    stopping = true;
    started = false;
    shutdownSignal.notify_all();
    thread.request_stop();
    stopRequested();
    //@todo kill ? join?

}

bool bsc::Runnable::isStopping() const {
//    std::unique_lock<std::mutex> g(stopMutex);
    return stopping;
}

void bsc::Runnable::join() {
    if (thread.joinable()) {
        thread.join();
    }
}

void bsc::Runnable::waitForStop() {
    std::unique_lock<std::mutex> g(stopMutex);
    //@todo C++20 wait on atomic
    shutdownSignal.wait(g, [this] { return stopping.load(); });
}
bool bsc::Runnable::isStarted() const {
    return started;
}
bool bsc::Runnable::isFinished() const {
    return finished;
}
