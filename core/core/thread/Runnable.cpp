#include <utility>

//
// Created by 23stilgar on 20.08.17.
//

#include "Runnable.h"
#include "RunnableThreadNotFinishedException.h"

void Runnable::start() {
    std::lock_guard g(startMutex);
    if (thread == nullptr) {
        if (Context::getActiveContext() == nullptr) {
            throw int(5);
        }
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
        throw RunnableThreadNotFinishedException();
    }


}

void Runnable::stop() {
    stopping = true;
    shutdownSignal.notify_all();
    //@todo kill ? join?

}

bool Runnable::isStopping() const {
    std::unique_lock<std::mutex> g(stopMutex);
    return stopping;
}

void Runnable::join() {
    if (thread != nullptr && thread->joinable())
        thread->join();
}

void Runnable::waitForStop() {
    std::unique_lock<std::mutex> g(stopMutex);
    shutdownSignal.wait(g, [this] { return stopping; });
}

