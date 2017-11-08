//
// Created by stilgar on 07.11.17.
//

#include <p2p/logic/sources/ClockSource.h>
#include <p2p/network/node/protocol/logic/actions/NodeActions.h>
#include "BasicModule.h"

BasicModule::BasicModule(INode &node) : NodeModule(node) {

}

void BasicModule::setupActions(LogicManager &logicManager) {


}

bool BasicModule::assignActions(LogicManager &logicManager) {
    return false;
}

bool BasicModule::setupSources(LogicManager &logicManager) {
    logicManager.addSource<ClockSource>();
    return true;
}
