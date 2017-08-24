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
        duration d = it.first;
        if (getLastTick(d) < now - d) {
            tick.setEventId(it.first);
            setLastTick(d,now);
            event<Tick>(tick);
        }
    }

    std::this_thread::sleep_for(10ms);

}
