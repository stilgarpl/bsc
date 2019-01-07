//
// Created by stilgar on 03.09.17.
//

#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/info/PacketInfo.h>
#include "TransmissionControl.h"


void TransmissionControl::onPacketReceived(const PacketEvent &event) {

    std::lock_guard<std::mutex> g(controlLock);
    const BasePacketPtr &packet = event.getPacket();
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
            packetsWaitingToAck[packet->getId()] = std::make_shared<BasePacketInfo>(packet, event.getConnection(),
                                                                                    Tick::clock::now());

        }
            break;

        case Status::ERROR :
            break;
    }
}

void TransmissionControl::onPacketSent(const PacketEvent &event) {
    std::lock_guard<std::mutex> g(controlLock);
    const BasePacketPtr packetPtr = event.getPacket();
    //  NODECONTEXTLOGGER("on PacketSent" + std::to_string(packetPtr->getId()));
    if (packetPtr != nullptr) {
        //@todo store sent time
        switch (packetPtr->getStatus()) {
            case Status::REQUEST: {
                if (!packetPtr->isRetry()) {
                    //  NODECONTEXTLOGGER("S-REQUEST " + std::to_string(packetPtr->getId()));

                    //@todo pointer or something else
                    packetsWaitingForAck[packetPtr->getId()] = std::make_shared<BasePacketInfo>(packetPtr,
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
    std::lock_guard<std::mutex> g(controlLock);
    //  std::clog << " TransControl::" << __func__ << std::endl;


    for (auto &&it : packetsWaitingForAck) {
        //@todo segfault here, probably from second/it being deleted in another event
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

                BasePacketPtr ackPacket = PacketUtils::getNewPtr<Status::ACK>(it.second->getPacketPtr());
                //ackPacket->setId(it.second->getPacketPtr()->getId());
                //ackPacket->setStatus(Status::ACK);
                NODECONTEXTLOGGER("sending response " + std::to_string(ackPacket->getId()));

                it.second->getConnection()->send(ackPacket);
            }
        }
    }
}

