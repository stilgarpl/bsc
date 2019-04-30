//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_BASICMODULE_H
#define BASYCO_BASICMODULE_H


#include <p2p/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>
#include <p2p/logic/sources/TriggerSource.h>


class BasicModule : public NodeModuleDependent<BasicModule> {
public:
    BasicModule(INode &node);

    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

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


};


#endif //BASYCO_BASICMODULE_H
