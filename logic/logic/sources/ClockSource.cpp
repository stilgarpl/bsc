//
// Created by Krzysztof Tulidowicz on 23.08.17.
//

#include <thread>
#include "../SourceManager.h"
#include "ClockSource.h"


using namespace std::chrono_literals;

bsc::ClockSource::TimePoint bsc::ClockSource::getLastTick(ClockSource::Duration d) {
    if (lastTick.count(d) == 0) {
        lastTick[d] = Clock::now();
    }
    return lastTick[d];
}

void bsc::ClockSource::setLastTick(ClockSource::Duration d, ClockSource::TimePoint t) {
    lastTick[d] = t;
}

bsc::ClockSource::ClockSource(SourceManager& sourceManager) : ISource(sourceManager) {}

void bsc::ClockSource::run() {
    while (!this->isStopping()) {
        bsc::Tick tick;
        //ticks
        auto now = Clock::now();
        tick.setNow(now);
        TimePoint nextTickTime = Clock::time_point::max();

        for (auto&& it : sourceManager.getSignalMap<bsc::Tick>()) {
            Duration d = it.first;
            if (getLastTick(d) < now - d) {
                tick.setEventId(it.first);
                setLastTick(d, now);
                event<bsc::Tick>(tick);
            }
            TimePoint timeToNextTick = getLastTick(d) + d;
            nextTickTime             = std::min(nextTickTime, timeToNextTick);
        }
        //@todo maybe it should be sleeping on a conditional variable? so it's waken up when thread is being stopped.
        // workaround for now, maximum sleep time 1s
        std::this_thread::sleep_for(std::min<decltype(nextTickTime - now)>(nextTickTime - now, 1s));
    }
}
