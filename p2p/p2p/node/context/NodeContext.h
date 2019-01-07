//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_NODECONTEXTINFO_H
#define BASYCO_NODECONTEXTINFO_H


#include <p2p/modules/nodeNetworkModule/protocol/context/exception/NodeContextNotSet.h>
#include "p2p/node/Node.h"
#include "p2p/modules/nodeNetworkModule/protocol/context/IContextSetup.h"

class NodeContext {
public:


private:
    Node &node;
    NodeInfo &nodeInfo;
public:

    NodeContext(Node &node, NodeInfo &nodeInfo);

    Node &getNode() const;

    NodeInfo &getNodeInfo() const;

    //shortcut method for getting node from active context
    static Node &getNodeFromActiveContext() {

        //@todo throw exeption if no NodeContext in context
        auto ptr = Context::getActiveContext()->get<NodeContext>();
        if (ptr != nullptr) {
            return ptr->getNode();
        } else {
            throw NodeContextNotSet();
        }
    }
};





#endif //BASYCO_NODECONTEXTINFO_H
