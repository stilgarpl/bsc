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

    explicit ModuleSource(SourceManager &sourceManager);

    template<typename ModuleType>
    void moduleStateChanged(const ModuleState &state, ModuleType &module) {
        ModuleEvent<ModuleType> event(module);
        event.setEventId(state);
        source.generateEvent<ModuleEvent<ModuleType>>(event);
    }

    void run() override;

    void onStop() override;

    void onStart() override;

};


#endif //BASYCO_MODULESOURCE_H
