//
// Created by stilgar on 03.09.17.
//

#include "TransmissionControl.h"

void TransmissionControl::onPacketReceived(const PacketEvent &event) {

    const NetworkPacketPtr &packet = event.getPacket();
    //  NODECONTEXTLOGGER("on PacketReceived"+ std::to_string(packet->getId()));

    //@todo compare time
    switch (packet->getStatus()) {
        case Status::RESPONSE :
        case Status::ACK:
            //      NODECONTEXTLOGGER("RESPONSE - ACK " + std::to_string(packet->getId()));

            waitingPackets.erase(packet->getId());

            break;
        case Status::REQUEST : {
            //      NODECONTEXTLOGGER("R-REQUEST " + std::to_string(packet->getId()));

            // sent ack or sth? but what if proper response is being prepared?
            //@todo send immediately? or wait for timeout?
            NetworkPacketPtr ackPacket = std::make_shared<NetworkPacket>();
            ackPacket->setId(packet->getId());
            ackPacket->setStatus(Status::ACK);
            //  NODECONTEXTLOGGER("sending response " + std::to_string(ackPacket->getId()));

            event.getConnection()->send(ackPacket);
        }
            break;

        case Status::ERROR :
            break;
    }
}

void TransmissionControl::onPacketSent(const PacketEvent &event) {
    const NetworkPacketPtr packet = event.getPacket();
    //  NODECONTEXTLOGGER("on PacketSent"+ std::to_string(packet->getId()));

    //@todo store sent time
    if (packet->getStatus() == Status::REQUEST && !packet->isRetry()) {
        // NODECONTEXTLOGGER("S-REQUEST " + std::to_string(packet->getId()));

        ///@todo pointer or something else
        waitingPackets[packet->getId()] = std::make_shared<NetworkPacketInfo>(packet, event.getConnection(),
                                                                              Tick::clock::now());
    }

}

void TransmissionControl::work(const Tick &tick) {
    //  std::clog << " TransControl::" << __func__ << std::endl;


    for (auto &&it : waitingPackets) {
        if (tick.getNow() - it.second->getTimeSent() > MAX_TIMEOUT) {
            if (it.second->getConnection() != nullptr) {
                //    NODECONTEXTLOGGER("Resending packet " + std::to_string(it.second->getPacketPtr()->getId()))
                it.second->getPacketPtr()->setRetry(true);
                it.second->getConnection()->send(it.second->getPacketPtr());
            }
        }
    }
}

