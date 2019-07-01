#include <utility>

//
// Created by stilgar on 07.10.17.
//

#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/network/NetworkModule.h>
#include "NetworkActions.h"


void NetworkActions::updateNetworkInfo(const NetworkInfoEvent &event) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        if (node.getModule<NetworkModule>()->getNetworkInfo() != nullptr &&
            node.getModule<NetworkModule>()->getNetworkInfo()->getNetworkId() ==
            event.getNetworkInfo().getNetworkId()) {
            //     LOGGER("SAME NETWORK! " + event.getNetworkInfo().getNetworkId());
            *node.getModule<NetworkModule>()->getNetworkInfo() += event.getNetworkInfo();

        }
        // NODECONTEXTLOGGER("received network info: " + event.getNetworkInfo().getNetworkId());
    }
}

void NetworkActions::saveNetworkInfo(const ModuleEvent<NetworkModule> &event) {
    Context::Ptr context = Context::getActiveContext();
//    LOGGER("SAVE NETWORK INFO")
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        node.getConfigurationManager().saveData<NetworkInfo>("networkInfo.dat", *event.getModule().getNetworkInfo());
    }
}

void NetworkActions::loadNetworkInfo(const ModuleEvent<NetworkModule> &event) {
    Context::Ptr context = Context::getActiveContext();
//    LOGGER("LOAD NETWORK INFO")
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        try {
            event.getModule().getNetworkInfo() = std::make_shared<NetworkInfo>(
                    node.getConfigurationManager().loadData<NetworkInfo>("networkInfo.dat"));
        } catch (DataFileNotFoundException &e) {
            // data file not found
            LOGGER("network data file not found")
        } catch (cereal::Exception &e) {
            LOGGER("cereal error during file load " + std::string(e.what()));
        }
    }
}

void NetworkActions::broadcastPacket(BasePacketPtr packet) {
    Context::Ptr context = Context::getActiveContext();
    LOGGER("BROADCAST PACKET")
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        //@todo null handle
        node.getModule<NetworkModule>()->broadcastPacket(std::move(packet));
    }

}

