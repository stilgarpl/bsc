//
// Created by stilgar on 05.09.17.
//

#include "IProtocol.h"


ProtocolWrapper::ProtocolWrapper(Connection *connection, IProtocol *protocol) : connection(connection),
                                                                                protocol(protocol) {}

std::future<NetworkPacketPtr> ProtocolWrapper::send(NetworkPacketPtr p) {
    //return protocol->send(connection,p);
    return std::future<NetworkPacketPtr>();
}

void DummyProtocol::onPacketSent(Context &context, const PacketEvent &event) {
    LOGGER("on packet sent")
}

void DummyProtocol::onPacketReceived(Context &context, const PacketEvent &event) {
    LOGGER("on packet received")
}

void DummyProtocol::work(Context &context, const Tick &tick) {
    LOGGER("on work")
}

std::future<NetworkPacketPtr> DummyProtocol::send(Connection *conn, NetworkPacketPtr p) {
    conn->send(p);
    return std::future<NetworkPacketPtr>();

}

void IProtocol::setupLogic(LogicManager &logicManager) {
    logicManager.setAction<PacketEvent>(Actions::onPacketSent, std::bind(&IProtocol::onPacketSent, this, _1, _2));
    logicManager.setAction<PacketEvent>(Actions::onPacketReceived,
                                        std::bind(&IProtocol::onPacketReceived, this, _1, _2));
    logicManager.setAction<Tick>(Actions::onWork, std::bind(&IProtocol::work, this, _1, _2));

    bool result = true;

    result &= logicManager.assignAction<PacketEvent>(PacketEvent::IdType::PACKET_SENT, Actions::onPacketSent);
    result &= logicManager.assignAction<PacketEvent>(PacketEvent::IdType::PACKET_RECEIVED, Actions::onPacketReceived);
    result &= logicManager.assignAction<Tick>(800ms, Actions::onWork);
    if (!result) {
        LOGGER("Something wasn't assigned correctly");
    } else {
        LOGGER("Protocol is assigned!")
    }

}
