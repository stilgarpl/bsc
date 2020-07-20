//
// Created by Krzysztof Tulidowicz on 19.07.18.
//

#include <p2p/modules/network/protocol/packet/NetworkInfoRequest.h>
#include "AuthModule.h"

bsc::AuthModule::AuthModule(bsc::INode& node) : NodeModuleDependent<AuthModule>(node, "auth") {}

void bsc::AuthModule::setupActions(LogicObject::SetupActionHelper& actionHelper) {
    when(state<bsc::ILogicModule>(bsc::ModuleState::SUBMODULES_PREPARED).entered()).fireStateChangeReaction(
            [&](bsc::ILogicModule& module) {
                //@todo move this mechanism to NodeModule to auto collect all submodules from other modules.
                //  submodules probably have to be optional or sth.
                auto& authSub = module.getSubModule<AuthModule>();
                LOGGER("GETTING AUTH SUB " + std::to_string(authSub.a) + "   " + node.getNodeInfo().getNodeId() + " " +
                       typeid(module).name())
                authSub.applyRules();
            });
}

bool bsc::AuthModule::assignActions(LogicObject::AssignActionHelper& actionHelper) {
    return true;
}

bool bsc::AuthModule::setupSources(LogicObject::SetupSourceHelper& sourceHelper) {
    return true;
}

void bsc::AuthModule::prepareSubmodules() {
    LOGGER("PREPARING SUBMODULE AUTH")
    auto& sub = getOwnSubModule();
    sub.requiredRoles<bsc::NetworkInfoRequest>("user");
//    sub.rule<NetworkInfoRequest>();
}

