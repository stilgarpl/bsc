//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_NODENETWORKMODULE_H
#define BASYCO_NODENETWORKMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>

class NodeNetworkModule : public NodeModule, public DependencyManaged<NodeNetworkModule> {
public:
    NodeNetworkModule(INode &node);

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;

};


#endif //BASYCO_NODENETWORKMODULE_H
