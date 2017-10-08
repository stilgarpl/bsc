//
// Created by stilgar on 06.10.17.
//

#include <log/Logger.h>
#include <network/node/protocol/logic/sources/NetworkSource.h>
#include <network/protocol/context/LogicContext.h>
#include "NetworkInfoResponse.h"


void NetworkInfoResponse::process(Context &context) {
    BasePacket::process(context);


    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto nodeSource = lc->getLogicManager().getSource<NetworkSource>();
        if (this->getNetworkInfo() != nullptr) {
            nodeSource->networkInfoReceived(*this->getNetworkInfo());
        } else {
            NODECONTEXTLOGGER("Empty network response")
        }
    }


    NODECONTEXTLOGGER("Network response received " + std::to_string(this->getId()) + " " +
                      this->getNetworkInfo()->getNetworkId());


}

NetworkInfoResponse::NetworkInfoResponse() {

    this->setStatus(Status::RESPONSE);
}
