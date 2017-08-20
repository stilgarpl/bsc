//
// Created by stilgar on 20.08.17.
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
    if (thread != nullptr) {
        thread->join();
    }


}

void Runnable::stop() {
    stopping = true;

}

bool Runnable::isStopping() const {
    return stopping;
}
