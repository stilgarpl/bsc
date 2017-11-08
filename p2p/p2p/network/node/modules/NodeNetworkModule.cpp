//
// Created by stilgar on 07.11.17.
//

#include <p2p/network/node/protocol/logic/sources/NodeSource.h>
#include <p2p/network/node/protocol/logic/sources/NetworkSource.h>
#include <p2p/network/protocol/logic/sources/ConnectionSource.h>
#include <p2p/network/protocol/logic/sources/AuthSource.h>
#include <p2p/network/node/protocol/logic/actions/NodeActions.h>
#include <p2p/network/protocol/logic/actions/ProtocolActions.h>
#include <p2p/network/node/protocol/logic/actions/NetworkActions.h>
#include "NodeNetworkModule.h"
#include "BasicModule.h"

NodeNetworkModule::NodeNetworkModule(INode &node) : NodeModule(node) {
    setRequired<BasicModule>();
}

void NodeNetworkModule::setupActions(LogicManager &logicManager) {
    logicManager.setAction<ConnectionEvent>("reqNoI", NodeActions::sendNodeInfoRequest);
    logicManager.setAction<ConnectionEvent>("reqNeI", NodeActions::sendNetworkInfoRequest);
    logicManager.setAction<NodeInfoEvent>("upNoI", NodeActions::updateNodeInfo);
    logicManager.setAction<NetworkInfoEvent>("upNeI", NetworkActions::updateNetworkInfo);
    logicManager.setAction<NodeInfoEvent>("addKnownNode", NodeActions::addKnownNode);
    logicManager.setAction<Tick>("trigNodeUp", NodeActions::triggerUpdateNode);
    logicManager.setAction<NodeInfoEvent>("nodeDiscovered", NodeActions::newNodeDiscovered);


    logicManager.setAction<ConnectionEvent>("connDebug", [](const ConnectionEvent &event) {
        // std::clog << "Debug: connection event!" << std::endl;
    });
///@todo reenable
//    logicManager.setAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
//                                        [&transmissionControl](
//                                                const PacketEvent &packetEvent) {
//                                            return transmissionControl.onPacketReceived(packetEvent);
//                                        });
//    logicManager.setAction<PacketEvent>(PacketEventId::PACKET_SENT,
//                                        [&transmissionControl](const PacketEvent &packetEvent) {
//                                            return transmissionControl.onPacketSent(packetEvent);
//                                        });
//
//    logicManager.setAction<Tick>("TransTick", [&transmissionControl](const Tick &tick) {
//        return transmissionControl.work(tick);
//    });


    logicManager.setAction<ConnectionEvent>("onConnect", ProtocolActions::onNewConnection);


}

bool NodeNetworkModule::assignActions(LogicManager &logicManager) {
    if (logicManager.assignAction<ConnectionEvent>("connDebug")) {
        std::clog << "Debug: ConEv assignment!" << std::endl;

    }


//    if (logicManager.assignAction<ConnectionEvent>("onConnect")) {
//        std::clog << "Debug: onConn assignment!" << std::endl;
//
//    }

    if (logicManager.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED, "reqNoI")) {
        std::clog << "Debug: reqNoI assignment!" << std::endl;

    }

    if (logicManager.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED, "reqNeI")) {
        std::clog << "Debug: reqNeI assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "upNoI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    if (logicManager.assignAction<NetworkInfoEvent>(NetworkInfoEvent::IdType::NETWORK_INFO_RECEIVED, "upNeI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    ///@todo upNoI powinno cos takiego robic, addKnownNode powinien byc wywolany tylko w przypadku
    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "addKnownNode")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NEW_NODE_DISCOVERED, "nodeDiscovered")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }
}

bool NodeNetworkModule::setupSources(LogicManager &logicManager) {
    logicManager.addSource<AuthSource>();

    logicManager.addSource<ConnectionSource>();
    logicManager.addSource<NetworkSource>();
    logicManager.addSource<NodeSource>();
}
