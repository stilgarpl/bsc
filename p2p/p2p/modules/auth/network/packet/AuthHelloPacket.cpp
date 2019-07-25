//
// Created by stilgar on 19.07.18.
//

#include <logic/context/LogicContext.h>
#include <p2p/modules/auth/logic/sources/AuthSource.h>
#include "AuthHelloPacket.h"

void AuthHelloPacket::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    //get auth module

//    auto& node = NodeContext::getNodeFromActiveContext();
//    auto authModule = node.getModule<AuthModule>();
    //@todo you got a context passed to the func! why use active context? I know it's supposed to be the same, but make up your mind!
    //  either always use passed context or remove this argument and always use active context. 
    auto &lc = LogicContext::getLogicManagerFromActiveContext();
    auto &authSource = lc.requireSource<AuthSource>();
    authSource.hello(nodeId, authData);


}
