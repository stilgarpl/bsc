//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_NODECONTEXTINFO_H
#define BASYCO_NODECONTEXTINFO_H


#include <p2p/modules/network/protocol/context/exception/NodeContextNotSet.h>
#include "p2p/node/Node.h"


namespace bsc {
    class NodeContext {
    public:


    private:
        Node& node;
        NodeInfo& nodeInfo;
    public:

        NodeContext(Node& node, NodeInfo& nodeInfo);

        [[nodiscard]] Node& getNode() const;

        [[nodiscard]] NodeInfo& getNodeInfo() const;

        //shortcut method for getting node from active context
        static Node& getNodeFromActiveContext() {
            return Context::getActiveContext()->get<NodeContext>().getNode();
        }
    };
}


#endif //BASYCO_NODECONTEXTINFO_H
