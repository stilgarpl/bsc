//
// Created by stilgar on 27.10.18.
//

#ifndef BASYCO_MODULEEVENT_H
#define BASYCO_MODULEEVENT_H

#include <p2p/logic/IEvent.h>
#include <p2p/node/ModuleState.h>


template<typename ModuleType>
class ModuleEvent : public IEvent<ModuleState> {
public:
    typedef ModuleType Type;
private:
    ModuleType &module;
public:
    ModuleType &getModule() const {
        return module;
    }

    explicit ModuleEvent(ModuleType &module) : module(module) {}

    ModuleState getState() {
        return getEventId();
    }
};


#endif //BASYCO_MODULEEVENT_H
