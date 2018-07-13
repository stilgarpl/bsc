//
// Created by stilgar on 06.10.17.
//

#include <p2p/log/Logger.h>
#include <p2p/network/node/protocol/logic/sources/NetworkSource.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/LogicContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/NodeContext.h>
#include "NetworkInfoResponse.h"


void NetworkInfoResponse::process(Context::Ptr context) {
    BasePacket::process(context);


    auto lc = context->get<LogicContext>();
    if (lc != nullptr) {
        auto nodeSource = lc->getLogicManager().getSource<NetworkSource>();
        if (this->getNetworkInfo() != nullptr) {
            nodeSource->networkInfoReceived(*this->getNetworkInfo());
        } else {
            //  NODECONTEXTLOGGER("Empty network response")
        }
    }


    NODECONTEXTLOGGER("Network response received " + std::to_string(this->getId()) + " " +
                      this->getNetworkInfo()->getNetworkId());


}
