//
// Created by stilgar on 23.08.17.
//

#include <chrono>
#include "Tick.h"

const std::chrono::time_point<typename Tick::Clock>& Tick::getNow() const {
    return now;
}

void Tick::setNow(const std::chrono::time_point<typename Tick::Clock>& now) {
    Tick::now = now;
}

Tick::Tick() {}
