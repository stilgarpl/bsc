//
// Created by stilgar on 07.10.17.
//

#include <p2p/modules/nodeNetworkModule/protocol/context/NodeContext.h>
#include <p2p/modules/nodeNetworkModule/NodeNetworkModule.h>
#include "NetworkActions.h"


void NetworkActions::updateNetworkInfo(const NetworkInfoEvent &event) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        if (node.getModule<NodeNetworkModule>()->getNetworkInfo() != nullptr &&
            node.getModule<NodeNetworkModule>()->getNetworkInfo()->getNetworkId() ==
            event.getNetworkInfo().getNetworkId()) {
            //     LOGGER("SAME NETWORK! " + event.getNetworkInfo().getNetworkId());
            *node.getModule<NodeNetworkModule>()->getNetworkInfo() += event.getNetworkInfo();

        }
        // NODECONTEXTLOGGER("received network info: " + event.getNetworkInfo().getNetworkId());
    }
}

void NetworkActions::saveNetworkInfo(const NetworkInfoEvent &event) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        node.getConfigurationManager().saveData<NetworkInfo>("networkInfo.dat", event.getNetworkInfo());
    }
}

void NetworkActions::loadNetworkInfo() {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        //@todo handle failed load
        node.getModule<NodeNetworkModule>()->getNetworkInfo() = std::make_shared<NetworkInfo>(
                node.getConfigurationManager().loadData<NetworkInfo>("networkInfo.dat"));
    }
}
