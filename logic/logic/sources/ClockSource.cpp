//
// Created by stilgar on 23.08.17.
//

#include <thread>
#include "../SourceManager.h"
#include "ClockSource.h"


using namespace std::chrono_literals;

ClockSource::TimePoint ClockSource::getLastTick(ClockSource::Duration d) {
    if (lastTick.count(d) == 0) {
        lastTick[d] = Clock::now();
    }
    return lastTick[d];
}

void ClockSource::setLastTick(ClockSource::Duration d, ClockSource::TimePoint t) {
    lastTick[d] = t;
}

ClockSource::ClockSource(SourceManager &sourceManager) : ISource(sourceManager) {}

void ClockSource::run() {
    while (!this->isStopping()) {
        Tick tick;
        //ticks
        auto now = Clock::now();
        tick.setNow(now);
        TimePoint nextTickTime = Clock::time_point::max();

        for (auto &&it : sourceManager.getSignalMap<Tick>()) {
            Duration d = it.first;
            if (getLastTick(d) < now - d) {
                tick.setEventId(it.first);
                setLastTick(d, now);
                event<Tick>(tick);
            }
            TimePoint timeToNextTick = getLastTick(d) + d;
            nextTickTime = std::min(nextTickTime, timeToNextTick);
        }

        std::this_thread::sleep_for(nextTickTime - now);
    }

}
