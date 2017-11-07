//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_BASICMODULE_H
#define BASYCO_BASICMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>


class BasicModule : public NodeModule, public DependencyManaged<BasicModule> {
public:
    BasicModule(INode &node);

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;

};


#endif //BASYCO_BASICMODULE_H
