//
// Created by stilgar on 19.07.18.
//

#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/auth/AuthModule.h>
#include <p2p/node/context/LogicContext.h>
#include <p2p/modules/auth/logic/sources/AuthSource.h>
#include "AuthHelloPacket.h"

void AuthHelloPacket::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    //get auth module

//    auto& node = NodeContext::getNodeFromActiveContext();
//    auto authModule = node.getModule<AuthModule>();

    auto &lc = LogicContext::getLogicManagerFromActiveContext();
    auto &authSource = lc.requireSource<AuthSource>();
    authSource.hello(nodeId, authData);


}
