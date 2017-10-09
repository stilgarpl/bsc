//
// Created by 23stilgar on 20.08.17.
//

#include "Runnable.h"

void Runnable::start() {
    if (thread == nullptr) {
        thread = std::make_unique<std::thread>(std::ref(*this));
    }

}

void Runnable::operator()() {
    run();
}

Runnable::~Runnable() {

    //@todo kill the thread
    join();


}

void Runnable::stop() {
    stopping = true;
    ///@todo kill ? join?iliokl

}

bool Runnable::isStopping() const {
    return stopping;
}

void Runnable::join() {
    if (thread != nullptr && thread->joinable())
        thread->join();
}
