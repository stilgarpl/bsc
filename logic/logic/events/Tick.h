//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_TICK_H
#define BASYCO_TICK_H



#include <chrono>
#include "../IEvent.h"

/**
 * tick every setDirect amount of time
 */
class Tick : public IEvent<std::chrono::milliseconds> {
public:
    typedef std::chrono::steady_clock Clock;
    typedef std::chrono::milliseconds Duration;

private:
    std::chrono::time_point<Clock> now;

public:
    [[nodiscard]] const std::chrono::time_point<Clock>& getNow() const;

    void setNow(const std::chrono::time_point<Clock>& now);

    Tick();
};


#endif //BASYCO_TICK_H
