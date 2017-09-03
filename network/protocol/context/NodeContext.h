//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_NODECONTEXTINFO_H
#define BASYCO_NODECONTEXTINFO_H


#include "../../node/Node.h"
#include "IContextSetup.h"

class NodeContext {
public:


private:
    Node &node;
    NodeInfo &nodeInfo;
public:

    NodeContext(Node &node, NodeInfo &nodeInfo);

    Node &getNode() const;

    NodeInfo &getNodeInfo() const;

};


#endif //BASYCO_NODECONTEXTINFO_H
