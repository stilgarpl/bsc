//
// Created by stilgar on 03.09.17.
//

#include "TransmissionControl.h"
#include "../../context/NodeContext.h"

void TransmissionControl::onPacketReceived(const PacketEvent &event) {

    const NetworkPacketPtr &packet = event.getPacket();
    //  NODECONTEXTLOGGER("on PacketReceived"+ std::to_string(packet->getId()));
    //   LOGGER("TC::OPR" + std::to_string(packet->getId()));
    //@todo compare time
    switch (packet->getStatus()) {
        case Status::RESPONSE :
        case Status::ACK:
            //      NODECONTEXTLOGGER("RESPONSE - ACK " + std::to_string(packet->getId()));
            // LOGGER("TC::OPR erasing packet "+ std::to_string(packet->getId()) + " count: " +std::to_string(packetsWaitingForAck.size()));
            packetsWaitingForAck.erase(packet->getId());

            break;
        case Status::REQUEST : {
            //      NODECONTEXTLOGGER("R-REQUEST " + std::to_string(packet->getId()));

            // sent ack or sth? but what if proper response is being prepared?
            packetsWaitingToAck[packet->getId()] = std::make_shared<NetworkPacketInfo>(packet, event.getConnection(),
                                                                                       Tick::clock::now());

        }
            break;

        case Status::ERROR :
            break;
    }
}

void TransmissionControl::onPacketSent(const PacketEvent &event) {
    const NetworkPacketPtr packetPtr = event.getPacket();
    //  NODECONTEXTLOGGER("on PacketSent"+ std::to_string(packetPtr->getId()));
    if (packetPtr != nullptr) {
        //@todo store sent time
        switch (packetPtr->getStatus()) {
            case Status::REQUEST: {
                if (!packetPtr->isRetry()) {
                    NODECONTEXTLOGGER("S-REQUEST " + std::to_string(packetPtr->getId()));

                    ///@todo pointer or something else
                    packetsWaitingForAck[packetPtr->getId()] = std::make_shared<NetworkPacketInfo>(packetPtr,
                                                                                                   event.getConnection(),

                                                                                                   Tick::clock::now());
                }
            }
                break;

            case Status::RESPONSE:
            case Status::ACK: {
                packetsWaitingToAck.erase(packetPtr->getId());

            }
                break;

            case Status::ERROR:
                break;
        }
    }

}

void TransmissionControl::work(const Tick &tick) {
    //  std::clog << " TransControl::" << __func__ << std::endl;


    for (auto &&it : packetsWaitingForAck) {
        if (tick.getNow() - it.second->getTimeSent() > MAX_TIMEOUT) {
            if (it.second->getConnection() != nullptr) {
                LOGGER("Resending packet " + std::to_string(it.second->getPacketPtr()->getId()))
                it.second->getPacketPtr()->setRetry(true);
                it.second->getConnection()->send(it.second->getPacketPtr());
            }
        }
    }

    for (auto &&it  : packetsWaitingToAck) {
        if (tick.getNow() - it.second->getTimeSent() > MAX_TIMEOUT) {
            if (it.second->getConnection() != nullptr) {
                //@todo send immediately? or wait for timeout?
                NetworkPacketPtr ackPacket = std::make_shared<BasePacket>();
                ackPacket->setId(it.second->getPacketPtr()->getId());
                ackPacket->setStatus(Status::ACK);
                //  NODECONTEXTLOGGER("sending response " + std::to_string(ackPacket->getId()));

                it.second->getConnection()->send(ackPacket);
            }
        }
    }
}

