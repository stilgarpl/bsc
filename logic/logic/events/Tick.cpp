//
// Created by Krzysztof Tulidowicz on 23.08.17.
//

#include <chrono>
#include "Tick.h"

const std::chrono::time_point<typename bsc::Tick::Clock>& bsc::Tick::getNow() const {
    return now;
}

void bsc::Tick::setNow(const std::chrono::time_point<typename Tick::Clock>& now) {
    Tick::now = now;
}

bsc::Tick::Tick() {}
