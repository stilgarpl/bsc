//
// Created by stilgar on 21.08.17.
//

#ifndef BSC_NODECONTEXTINFO_H
#define BSC_NODECONTEXTINFO_H


#include "p2p/core/node/Node.h"


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
            return Context::getActiveContext()->get<NodeContext>()->getNode();
        }
    };
}


#endif //BSC_NODECONTEXTINFO_H
