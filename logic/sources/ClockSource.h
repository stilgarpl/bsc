//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_CLOCKSOURCE_H
#define BASYCO_CLOCKSOURCE_H


#include "../ISource.h"

class ClockSource : public ISource {
public:
    void work() override;
};


#endif //BASYCO_CLOCKSOURCE_H
