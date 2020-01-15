//
// Created by stilgar on 27.10.18.
//

#ifndef BASYCO_MODULEEVENT_H
#define BASYCO_MODULEEVENT_H

#include <p2p/node/module/ModuleState.h>
#include "logic/IEvent.h"



template<typename ModuleType>
class ModuleEvent : public bsc::IEvent<ModuleState> {
public:
    typedef ModuleType Type;
private:
    ModuleType& module;
public:
    ModuleType& getModule() const {
        return module;
    }

    explicit ModuleEvent(ModuleType& module) : module(module) {}

    ModuleState getState() {
        return getEventId();
    }
};


#endif //BASYCO_MODULEEVENT_H
