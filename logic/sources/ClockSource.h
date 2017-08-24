//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_CLOCKSOURCE_H
#define BASYCO_CLOCKSOURCE_H


#include "../ISource.h"

class ClockSource : public ISource {
    typedef std::chrono::steady_clock clock;
    typedef std::chrono::time_point<clock> time_point;
    typedef Tick::duration duration;
protected:
    time_point getLastTick(duration)
public:
    void work() override;
};


#endif //BASYCO_CLOCKSOURCE_H
