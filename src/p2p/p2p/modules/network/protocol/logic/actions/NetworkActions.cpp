#include <utility>

//
// Created by Krzysztof Tulidowicz on 07.10.17.
//

#include <p2p/core/node/context/NodeContext.h>
#include <p2p/modules/network/NetworkModule.h>
#include "NetworkActions.h"


void bsc::NetworkActions::updateNetworkInfo(const NetworkInfoEvent& event) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();

    auto& node = nodeContext->getNode();
    if (node.getModule<NetworkModule>()->getNetworkInfo() != nullptr &&
        node.getModule<NetworkModule>()->getNetworkInfo()->getNetworkId() ==
        event.getNetworkInfo().getNetworkId()) {
        //     logger.debug("SAME NETWORK! " + event.getNetworkInfo().getNetworkId());
        *node.getModule<NetworkModule>()->getNetworkInfo() += event.getNetworkInfo();

    }
    // logger.debug("received network info: " + event.getNetworkInfo().getNetworkId());
}

void bsc::NetworkActions::saveNetworkInfo(const ModuleEvent<NetworkModule>& event) {
    Context::Ptr context = Context::getActiveContext();
    //    logger.debug("SAVE NETWORK INFO");
    auto nodeContext = context->get<NodeContext>();

    auto& node = nodeContext->getNode();
    if (event.getModule().getNetworkInfo() != nullptr) {
        node.getConfigurationManager().saveData<NetworkInfo>("networkInfo.dat", *event.getModule().getNetworkInfo());
    }
}

void bsc::NetworkActions::loadNetworkInfo(const ModuleEvent<NetworkModule>& event) {
    Context::Ptr context = Context::getActiveContext();
    //    logger.debug("LOAD NETWORK INFO");
    auto nodeContext = context->get<NodeContext>();

    auto& node = nodeContext->getNode();
    try {
        event.getModule().getNetworkInfo() = std::make_shared<NetworkInfo>(
                node.getConfigurationManager().loadData<NetworkInfo>("networkInfo.dat"));
    } catch (DataFileNotFoundException& e) {
        // data file not found
        logger.debug("network data file not found");
    } catch (cereal::Exception &e) {
        logger.debug("cereal error during file load " + std::string(e.what()));
    }

}

void bsc::NetworkActions::broadcastPacket(BasePacketPtr packet) {
    Context::Ptr context = Context::getActiveContext();
    logger.debug("BROADCAST PACKET");
    auto nodeContext = context->get<NodeContext>();

    auto& node = nodeContext->getNode();
    //@todo null handle
    node.getModule<NetworkModule>()->broadcastPacket(std::move(packet));


}

