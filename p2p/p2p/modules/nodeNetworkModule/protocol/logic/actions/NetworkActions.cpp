#include <utility>

//
// Created by stilgar on 07.10.17.
//

#include <p2p/node/context/NodeContext.h>
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

void NetworkActions::saveNetworkInfo(const ModuleEvent<NodeNetworkModule> &event) {
    Context::Ptr context = Context::getActiveContext();
//    LOGGER("SAVE NETWORK INFO")
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        node.getConfigurationManager().saveData<NetworkInfo>("networkInfo.dat", *event.getModule().getNetworkInfo());
    }
}

void NetworkActions::loadNetworkInfo(const ModuleEvent<NodeNetworkModule> &event) {
    Context::Ptr context = Context::getActiveContext();
//    LOGGER("LOAD NETWORK INFO")
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        //@todo handle failed load
        event.getModule().getNetworkInfo() = std::make_shared<NetworkInfo>(
                node.getConfigurationManager().loadData<NetworkInfo>("networkInfo.dat"));
    }
}

void NetworkActions::broadcastPacket(BasePacketPtr packet) {
    Context::Ptr context = Context::getActiveContext();
    LOGGER("BROADCAST PACKET")
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        //@todo null handle
        node.getModule<NodeNetworkModule>()->broadcastPacket(std::move(packet));
    }

}

