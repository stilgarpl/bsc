//
// Created by stilgar on 27.10.18.
//

#ifndef BASYCO_MODULEEVENT_H
#define BASYCO_MODULEEVENT_H

#include <p2p/logic/IEvent.h>

enum class ModuleEventId {
    MODULE_INITIALIZED,
};

template<typename ModuleType>
class ModuleEvent : public IEvent<ModuleEventId> {
public:
    typedef ModuleType Type;
};


#endif //BASYCO_MODULEEVENT_H
