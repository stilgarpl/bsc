//
// Created by stilgar on 23.08.17.
//

#include <thread>
#include "ClockSource.h"
#include "../events/Tick.h"


using namespace std::chrono_literals;

void ClockSource::work() {

    Tick tick;
    //ticks
    for (auto &&it : getSignalMap<Tick>()) {

        tick.setEventId(it.first);
        event<Tick>(tick);
    }

    std::this_thread::sleep_for(1s);

}
