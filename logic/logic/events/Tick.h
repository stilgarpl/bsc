//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_TICK_H
#define BASYCO_TICK_H



#include <chrono>
#include "../IEvent.h"

/**
 * tick every set amount of time
 */
class Tick : public IEvent<std::chrono::milliseconds> {
public:
    typedef std::chrono::steady_clock clock;
    typedef std::chrono::milliseconds duration;

private:
    std::chrono::time_point<clock> now;

public:
    const std::chrono::time_point<clock> &getNow() const;

    void setNow(const std::chrono::time_point<clock> &now);

    Tick();
};


#endif //BASYCO_TICK_H
