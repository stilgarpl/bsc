//
// Created by stilgar on 06.10.17.
//

#include "NetworkInfoResponse.h"
#include "../logic/sources/NodeSource.h"
#include "../../../protocol/context/LogicContext.h"

void NetworkInfoResponse::process(Context &context) {
    BasePacket::process(context);


    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto nodeSource = lc->getLogicManager().getSource<NodeSource>();
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
