//
// Created by stilgar on 03.09.17.
//

#ifndef BASYCO_PACKETEVENT_H
#define BASYCO_PACKETEVENT_H

#include "../../../../logic/IEvent.h"
#include "../../packet/BasePacket.h"
#include "../../connection/Connection.h"

enum class PacketEventId {
    PACKET_SENT,
    PACKET_RECEIVED,
};

class PacketEvent : public IEvent<PacketEventId> {
private:
    ///@todo maybe merge with ConnectionEvent?
    NetworkPacketPtr packet;
    //@todo shared_ptr?
    Connection *connection;

public:
    const NetworkPacketPtr &getPacket() const;

    void setPacket(const NetworkPacketPtr &packet);

    Connection *getConnection() const;

    void setConnection(Connection *connection);

};


#endif //BASYCO_PACKETEVENT_H
