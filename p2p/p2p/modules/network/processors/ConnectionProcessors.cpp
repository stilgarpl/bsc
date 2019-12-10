//
// Created by stilgar on 10.12.2019.
//

#include <p2p/modules/network/protocol/context/ConnectionContext.h>
#include <p2p/modules/network/remote/RemoteNodeContext.h>
#include "ConnectionProcessors.h"

ConnectionControl::Response::Ptr
ConnectionProcessors::processConnectionControl(ConnectionControl::Request::Ptr request) {
    LOGGER("diconnection processor")
    auto remoteNodeContext = Context::getActiveContext()->get<RemoteNodeContext>();
    if (remoteNodeContext != nullptr) {
        LOGGER("sending disconnect")
        auto& remoteNode = remoteNodeContext->getRemoteNode();
        remoteNode.disconnect();
    } else {
        //@todo throw
    }


    return nullptr;
}
