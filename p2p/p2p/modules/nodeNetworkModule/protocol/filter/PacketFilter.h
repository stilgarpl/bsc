//
// Created by stilgar on 12.11.17.
//

#ifndef BASYCO_PACKETFILTER_H
#define BASYCO_PACKETFILTER_H


#include <p2p/modules/nodeNetworkModule/protocol/packet/BasePacket.h>

class PacketFilter {

public:
    virtual bool filter(BasePacketPtr packet) =0;

    virtual ~PacketFilter() = default;
};


#endif //BASYCO_PACKETFILTER_H
