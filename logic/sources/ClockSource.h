//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_CLOCKSOURCE_H
#define BASYCO_CLOCKSOURCE_H

#include "../events/Tick.h"
#include <chrono>
#include "../ISource.h"

class ClockSource : public ISource {
    typedef Tick::clock clock;
    typedef std::chrono::time_point<clock> time_point;
    typedef Tick::duration duration;
private:
    std::map<duration, time_point> lastTick;
protected:
    time_point getLastTick(duration d);;

    void setLastTick(duration d, time_point t);;

public:
    void work() override;

    void registerProviders(SourceManager *manager) override;
};


#endif //BASYCO_CLOCKSOURCE_H
