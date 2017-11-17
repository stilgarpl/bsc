//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_NODEMODULE_H
#define BASYCO_NODEMODULE_H


#include <p2p/logic/ILogicModule.h>
#include <p2p/dependency/IDependencyManaged.h>
#include "INode.h"
#include "INodeModule.h"

class NodeModule : public INodeModule {

protected:
    INode &node;
public:
    ///@todo pure or not?
    virtual void initialize() {};

    NodeModule(INode &node) : node(node) {}

    void run() override {
        //do nothing, just so modules do not have to implement this if they don't want to
    }
};


#endif //BASYCO_NODEMODULE_H
