//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_NODEMODULE_H
#define BASYCO_NODEMODULE_H


#include <p2p/logic/ILogicModule.h>
#include <p2p/dependency/IDependencyManaged.h>
#include "INode.h"

class NodeModule : public ILogicModule, public virtual IDependencyManaged {

private:
    INode &node;
public:
    ///@todo pure or not?
    virtual void initialize() {};

    NodeModule(INode &node) : node(node) {}
};


#endif //BASYCO_NODEMODULE_H
