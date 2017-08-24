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
    auto now = clock::now();
    for (auto &&it : getSignalMap<Tick>()) {
        duration duration = it.first;
        if (lastTick(duration) < now - duration) {
            tick.setEventId(it.first);
            event<Tick>(tick);
        }
    }

    std::this_thread::sleep_for(1s);

}
