//
// Created by stilgar on 27.10.18.
//

#ifndef BASYCO_MODULESOURCE_H
#define BASYCO_MODULESOURCE_H


#include <p2p/node/ModuleState.h>
#include <p2p/logic/events/ModuleEvent.h>
#include "AutoSource.h"

class ModuleSource : public ISource {

private:
    AutoSource source;

public:
    void work() override;

    explicit ModuleSource(SourceManager &sourceManager);

    template<typename ModuleType>
    void moduleStateChanged(const ModuleState &state, ModuleType &module) {
//        std::string txt;
//        switch (state) {
//
//            case ModuleState::INITIALIZED:
//                txt = "initizalized";
//                break;
//            case ModuleState::LOGIC_READY:
//                txt = "logic ready";
//                break;
//            case ModuleState::READY:
//                txt = "ready";
//                break;
//            case ModuleState::SHUTDOWN:
//                txt = "shutdown";
//                break;
//        }
//        LOGGER(std::string("Module state changed ") + typeid(ModuleType).name() + " " + txt)
        ModuleEvent<ModuleType> event(module);
        event.setEventId(state);
        source.generateEvent<ModuleEvent<ModuleType>>(event);
    }

};


#endif //BASYCO_MODULESOURCE_H
