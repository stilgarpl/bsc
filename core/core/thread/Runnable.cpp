#include <utility>
#include <exception>

//
// Created by 23stilgar on 20.08.17.
//

#include "Runnable.h"

void Runnable::start() {
    std::lock_guard g(startMutex);
    if (thread == nullptr) {
        thread = std::make_unique<std::thread>(std::ref(*this), Context::getActiveContext());
    }

}

void Runnable::operator()(Context::Ptr contextPtr) {
    Context::setActiveContext(std::move(contextPtr));
    onStart();
    run();
    onStop();
    finished = true;
}

Runnable::~Runnable() {
    if (!finished) {
        stop();
        join();
    }


}

void Runnable::stop() {
    std::unique_lock<std::mutex> g(stopMutex);
    stopping = true;
    shutdownSignal.notify_all();
    //@todo kill ? join?

}

bool Runnable::isStopping() const {
//    std::unique_lock<std::mutex> g(stopMutex);
    return stopping;
}

void Runnable::join() {
    if (thread != nullptr && thread->joinable())
        thread->join();
}

void Runnable::waitForStop() {
    std::unique_lock<std::mutex> g(stopMutex);
    //@todo C++20 wait on atomic
    shutdownSignal.wait(g, [this] { return stopping.load(); });
}

