//
// Created by stilgar on 23.08.17.
//

#include <thread>
#include "../SourceManager.h"
#include "ClockSource.h"


using namespace std::chrono_literals;

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
    while (!this->isStopping()) {
        Tick tick;
        //ticks
        auto now = clock::now();
        tick.setNow(now);
        time_point nextTickTime = clock::time_point::max();

        for (auto &&it : sourceManager.getSignalMap<Tick>()) {
            duration d = it.first;
            if (getLastTick(d) < now - d) {
                tick.setEventId(it.first);
                setLastTick(d, now);
                event<Tick>(tick);
            }
            time_point timeToNextTick = getLastTick(d) + d;
            nextTickTime = std::min(nextTickTime, timeToNextTick);
        }

        std::this_thread::sleep_for(nextTickTime - now);
    }

}
