//
// Created by stilgar on 03.09.17.
//

#include "TransmissionControl.h"

void TransmissionControl::onPacketReceived(Context &, const PacketEvent &event) {
    std::clog << __func__ << std::endl;
    const NetworkPacketPtr &packet = event.getPacket();
    //@todo compare time
    switch (packet->getStatus()) {
        case Status::RESPONSE :
        case Status::ACK:
            std::clog << "RESPONSE - ACK" << packet->getId() << std::endl;

            waitingPackets.erase(packet->getId());

            break;
        case Status::REQUEST : {
            std::clog << "R-REQUEST" << packet->getId() << std::endl;

            // sent ack or sth? but what if proper response is being prepared?
            //@todo send immediately? or wait for timeout?
            NetworkPacketPtr ackPacket = std::make_shared<NetworkPacket>();
            ackPacket->setId(packet->getId());
            ackPacket->setStatus(Status::ACK);
            event.getConnection()->send(ackPacket);
        }
            break;

        case Status::ERROR :
            break;
    }
}

void TransmissionControl::onPacketSent(Context &, const PacketEvent &event) {
    std::clog << __func__ << std::endl;
    const NetworkPacketPtr packet = event.getPacket();
    //@todo store sent time
    if (packet->getStatus() == Status::REQUEST) {
        std::clog << "S-REQUEST" << packet->getId() << std::endl;

        ///@todo pointer or something else
        waitingPackets[packet->getId()] = std::make_shared<NetworkPacketInfo>(packet, event.getConnection(),
                                                                              Tick::clock::now());
    }

}

void TransmissionControl::work(Context &, const Tick &tick) {
    //  std::clog << " TransControl::" << __func__ << std::endl;


    for (auto &&it : waitingPackets) {
        if (tick.getNow() - it.second->getTimeSent() > MAX_TIMEOUT) {
            if (it.second->getConnection() != nullptr) {
                std::clog << " Resending packet " << std::endl;
                it.second->getConnection()->send(it.second->getPacketPtr());
            }
        }
    }
}

NetworkPacketInfo::NetworkPacketInfo(const NetworkPacketPtr &packetPtr,
                                     const std::chrono::time_point<Tick::clock> &timeSent)
        : packetPtr(packetPtr), timeSent(timeSent) {}

const NetworkPacketPtr &NetworkPacketInfo::getPacketPtr() const {
    return packetPtr;
}

void NetworkPacketInfo::setPacketPtr(const NetworkPacketPtr &packetPtr) {
    NetworkPacketInfo::packetPtr = packetPtr;
}

const std::chrono::time_point<Tick::clock> &NetworkPacketInfo::getTimeSent() const {
    return timeSent;
}

void NetworkPacketInfo::setTimeSent(const std::chrono::time_point<Tick::clock> &timeSent) {
    NetworkPacketInfo::timeSent = timeSent;
}

NetworkPacketInfo::NetworkPacketInfo(const NetworkPacketPtr &packetPtr, Connection *connection,
                                     const std::chrono::time_point<Tick::clock> &timeSent) : packetPtr(packetPtr),
                                                                                             connection(connection),
                                                                                             timeSent(timeSent) {}

Connection *NetworkPacketInfo::getConnection() const {
    return connection;
}

void NetworkPacketInfo::setConnection(Connection *connection) {
    NetworkPacketInfo::connection = connection;
}
