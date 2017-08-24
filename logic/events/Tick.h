//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_TICK_H
#define BASYCO_TICK_H


#include "../IEvent.h"

/**
 * tickuje co int czasu
 */
class Tick : public IEvent<std::chrono::milliseconds> {
public:
    typedef std::chrono::milliseconds duration;
};


#endif //BASYCO_TICK_H
