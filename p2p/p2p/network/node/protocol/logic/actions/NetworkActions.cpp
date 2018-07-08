//
// Created by stilgar on 07.10.17.
//

#include <p2p/network/protocol/context/NodeContext.h>
#include "NetworkActions.h"

void NetworkActions::updateNetworkInfo(const NetworkInfoEvent &event) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        if (node.getNetworkInfo() != nullptr &&
            node.getNetworkInfo()->getNetworkId() == event.getNetworkInfo().getNetworkId()) {
            //     LOGGER("SAME NETWORK! " + event.getNetworkInfo().getNetworkId());
            *node.getNetworkInfo() += event.getNetworkInfo();

        }
        // NODECONTEXTLOGGER("received network info: " + event.getNetworkInfo().getNetworkId());
    }
}