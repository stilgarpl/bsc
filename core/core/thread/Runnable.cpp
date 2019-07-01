#include <utility>

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
}

Runnable::~Runnable() {

    //@todo kill the thread
    stop();
    //@todo wait time?
    std::this_thread::sleep_for(5ms);
    join();


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

