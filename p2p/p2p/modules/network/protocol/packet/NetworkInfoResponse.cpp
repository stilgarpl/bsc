//
// Created by stilgar on 06.10.17.
//

#include <core/log/Logger.h>
#include <p2p/modules/network/protocol/logic/sources/NetworkSource.h>
#include <logic/context/LogicContext.h>
#include <p2p/node/context/NodeContext.h>
#include "NetworkInfoResponse.h"


void NetworkInfoResponse::process(Context::Ptr context) {
    BasePacket::process(context);


    auto& lc = context->get<LogicContext>();

        auto nodeSource = lc.getLogicManager().getSource<NetworkSource>();
        if (this->getNetworkInfo() != nullptr) {
            nodeSource->networkInfoReceived(*this->getNetworkInfo());
        } else {
            //  LOGGER("Empty network response")
        }



    LOGGER("Network response received " + std::to_string(this->getId()) + " " +
           this->getNetworkInfo()->getNetworkId());


}
