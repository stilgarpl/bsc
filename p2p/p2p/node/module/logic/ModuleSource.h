//
// Created by stilgar on 27.10.18.
//

#ifndef BSC_MODULESOURCE_H
#define BSC_MODULESOURCE_H


#include <p2p/node/module/ModuleState.h>
#include "p2p/node/module/logic/events/ModuleEvent.h"


#include <logic/sources/AutoSource.h>


namespace bsc {
    class ModuleSource : public bsc::ISource {

    private:
        AutoSource source;

    public:

        explicit ModuleSource(SourceManager& sourceManager);

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
}


#endif //BSC_MODULESOURCE_H
