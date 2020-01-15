//
// Created by stilgar on 27.10.18.
//

#ifndef BASYCO_MODULESOURCE_H
#define BASYCO_MODULESOURCE_H


#include <p2p/node/module/ModuleState.h>
#include "p2p/node/module/logic/events/ModuleEvent.h"
#include <logic/sources/AutoSource.h>


class ModuleSource : public bsc::ISource {

private:
    bsc::AutoSource source;

public:

    explicit ModuleSource(bsc::SourceManager& sourceManager);

    template<typename ModuleType>
    void moduleStateChanged(const ModuleState& state, ModuleType& module) {
        ModuleEvent<ModuleType> event(module);
        event.setEventId(state);
        source.generateEvent<ModuleEvent<ModuleType>>(event);
    }

    void run() override;

    void onStop() override;

    void onStart() override;

};


#endif //BASYCO_MODULESOURCE_H
