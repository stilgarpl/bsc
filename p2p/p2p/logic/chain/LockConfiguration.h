//
// Created by stilgar on 4/18/19.
//

#ifndef BASYCO_LOCKCONFIGURATION_H
#define BASYCO_LOCKCONFIGURATION_H


#include "ChainDefinitions.h"

class LockConfiguration {
public:
    virtual ChainLockIdType getLockId() = 0;

};


#endif //BASYCO_LOCKCONFIGURATION_H
