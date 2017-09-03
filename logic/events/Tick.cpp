//
// Created by stilgar on 23.08.17.
//

#include "../../../../../../usr/lib/gcc/x86_64-pc-linux-gnu/5.4.0/include/g++-v5/chrono"
#include "Tick.h"

const std::chrono::time_point<typename Tick::clock> &Tick::getNow() const {
    return now;
}

void Tick::setNow(const std::chrono::time_point<typename Tick::clock> &now) {
    Tick::now = now;
}
