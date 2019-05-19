//
// Created by stilgar on 19.07.18.
//

#include <p2p/modules/nodeNetworkModule/protocol/packet/NetworkInfoRequest.h>
#include "AuthModule.h"

AuthModule::AuthModule(INode &node) : NodeModuleDependent<AuthModule>(node, "auth") {}

void AuthModule::setupActions(LogicObject::SetupActionHelper &actionHelper) {
    when(state<ILogicModule>(ModuleState::SUBMODULES_PREPARED).entered()).fireStateChangeReaction(
            [&](ILogicModule &module) {
                //@todo move this mechanism to NodeModule to auto collect all submodules from other modules.
                //  submodules probably have to be optional or sth.
                auto &authSub = module.getSubModule<AuthModule>();
                LOGGER("GETTING AUTH SUB " + std::to_string(authSub.a) + "   " + node.getNodeInfo().getNodeId() + " " +
                       typeid(module).name())
                authSub.applyRules();
            });
}

bool AuthModule::assignActions(LogicObject::AssignActionHelper &actionHelper) {
    return true;
}

bool AuthModule::setupSources(LogicObject::SetupSourceHelper &sourceHelper) {
    return true;
}

void AuthModule::prepareSubmodules() {
    LOGGER("PREPARING SUBMODULE AUTH")
    auto &sub = getOwnSubModule();
    sub.requiredRoles<NetworkInfoRequest>("user");
//    sub.rule<NetworkInfoRequest>();
}

