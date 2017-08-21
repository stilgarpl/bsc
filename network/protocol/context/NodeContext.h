//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_NODECONTEXTINFO_H
#define BASYCO_NODECONTEXTINFO_H


#include "../../node/Node.h"
#include "IContextSetup.h"

class NodeContext {
public:
    class Setup : public IContextSetup {
    private:
        Node &node;
        NodeInfo &nodeInfo;
    public:
        void setup(Context &context) override;

        Setup(Node &node, NodeInfo &nodeInfo);
    };

private:
    Node &node;
    NodeInfo &nodeInfo;
public:

    NodeContext(Node &node, NodeInfo &nodeInfo);
    //NodeContext(NodeInfo &nodeInfo);
};


#endif //BASYCO_NODECONTEXTINFO_H
