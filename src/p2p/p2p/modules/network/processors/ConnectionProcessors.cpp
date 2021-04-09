//
// Created by Krzysztof Tulidowicz on 10.12.2019.
//

#include <p2p/modules/network/protocol/context/ConnectionContext.h>
#include <p2p/modules/network/remote/RemoteNodeContext.h>
#include "ConnectionProcessors.h"

namespace bsc {
    ConnectionControl::Response::Ptr
    ConnectionProcessors::processConnectionControl(ConnectionControl::Request::Ptr request) {
        logger.debug("diconnection processor");
        Context::getActiveContext()->get<RemoteNodeContext>()->getRemoteNode().disconnect();

        return nullptr;
    }
}