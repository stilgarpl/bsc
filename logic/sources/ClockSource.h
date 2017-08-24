//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_CLOCKSOURCE_H
#define BASYCO_CLOCKSOURCE_H

#include "../events/Tick.h"
#include <chrono>
#include "../ISource.h"

class ClockSource : public ISource {
    typedef std::chrono::steady_clock clock;
    typedef std::chrono::time_point<clock> time_point;
    typedef Tick::duration duration;
private:
    std::map<duration,time_point> lastTick;
protected:
    time_point getLastTick(duration d) {
        if (lastTick.count(d) == 0 ) {
                lastTick[d]=clock::now();
            }
        return lastTick[d];
    };
    void setLastTick(duration d, time_point t) {
        lastTick[d] = t;
    };
    
public:
    void work() override;
};


#endif //BASYCO_CLOCKSOURCE_H
