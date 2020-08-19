//
// Created by Krzysztof Tulidowicz on 07.11.17.
//

#ifndef BSC_BASICMODULE_H
#define BSC_BASICMODULE_H


#include <p2p/core/node/module/NodeModule.h>
#include <p2p/core/dependency/DependencyManaged.h>
#include <logic/sources/TriggerSource.h>

namespace bsc {
    class BasicModule : public NodeModuleDependent<BasicModule> {
    public:
        BasicModule(INode& node);

        void setupActions(SetupActionHelper& actionHelper) override;

        bool assignActions(AssignActionHelper& actionHelper) override;

        bool setupSources(SetupSourceHelper& sourceHelper) override;

    public:
        ////////////////////////////////
        /// Commands section
        ////////////////////////////////

        void shutdownNode() {
            node.stop();
        }

        //@todo not sure if it belongs here: (fireTriggers)

        template<typename TriggerIdType>
        void fireTrigger(TriggerIdType id) {
            node.getLogicManager().getSource<TriggerSource>()->fireTrigger<TriggerIdType>(id);
        }

        template<typename TriggerIdType, typename TriggerValueType>
        void fireTriggerValue(TriggerIdType id, TriggerValueType value) {
            node.getLogicManager().getSource<TriggerSource>()->fireTrigger<TriggerIdType, TriggerValueType>(id, value);
        }

        ///IMPORTANT: DO NOT NAME COMMANDS LIKE THE METHOD THEY CALL FROM OTHER MODULES. I HAD THIS PROBLEM WITH SHUTDOWN, STOP AND SAVECONFIGURATION.


        void saveAllConfiguration() {
            node.saveConfiguration();
        }


    };
}


#endif //BSC_BASICMODULE_H
