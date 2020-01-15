#include <utility>
#include <exception>

//
// Created by 23stilgar on 20.08.17.
//

#include "Runnable.h"


void bsc::Runnable::start() {
    std::lock_guard g(startMutex);
    if (thread == nullptr) {
        thread = std::make_unique<std::thread>(std::ref(*this), bsc::Context::getActiveContext());
    }

}

void bsc::Runnable::operator()(bsc::Context::Ptr contextPtr) {
    bsc::Context::setActiveContext(std::move(contextPtr));
    onStart();
    run();
    onStop();
    finished = true;
}

bsc::Runnable::~Runnable() {
    if (!finished) {
        stop();
        join();
    }


}

void bsc::Runnable::stop() {
    std::unique_lock<std::mutex> g(stopMutex);
    stopping = true;
    shutdownSignal.notify_all();
    //@todo kill ? join?

}

bool bsc::Runnable::isStopping() const {
//    std::unique_lock<std::mutex> g(stopMutex);
    return stopping;
}

void bsc::Runnable::join() {
    if (thread != nullptr && thread->joinable())
        thread->join();
}

void bsc::Runnable::waitForStop() {
    std::unique_lock<std::mutex> g(stopMutex);
    //@todo C++20 wait on atomic
    shutdownSignal.wait(g, [this] { return stopping.load(); });
}

