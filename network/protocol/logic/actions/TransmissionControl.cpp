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
            waitingPackets.erase(packet->getId());

            break;
        case Status::REQUEST : {
            // sent ack or sth? but what if proper response is being prepared?
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
        waitingPackets[packet->getId()] = packet;
    }

}
