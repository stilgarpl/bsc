//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_BASICMODULE_H
#define BASYCO_BASICMODULE_H


#include <p2p/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>


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

    void shutdown() {
        node.stop();
    }


};


#endif //BASYCO_BASICMODULE_H
