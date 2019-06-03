//
// Created by stilgar on 23.08.17.
//

#include <thread>
#include <p2p/logic/SourceManager.h>
#include "ClockSource.h"


using namespace std::chrono_literals;

void ClockSource::work() {

    Tick tick;
    //ticks
    auto now = clock::now();
    tick.setNow(now);


    for (auto &&it : sourceManager.getSignalMap<Tick>()) {
        duration d = it.first;
        if (getLastTick(d) < now - d) {
            tick.setEventId(it.first);
            setLastTick(d, now);
            event<Tick>(tick);
        }
    }

    std::this_thread::sleep_for(1ms);

}

ClockSource::time_point ClockSource::getLastTick(ClockSource::duration d) {
    if (lastTick.count(d) == 0) {
        lastTick[d] = clock::now();
    }
    return lastTick[d];
}

void ClockSource::setLastTick(ClockSource::duration d, ClockSource::time_point t) {
    lastTick[d] = t;
}

ClockSource::ClockSource(SourceManager &sourceManager) : ISource(sourceManager) {}

void ClockSource::run() {
//@todo implement actual version
    while (!this->isStopping()) {
        work();
    }

}
