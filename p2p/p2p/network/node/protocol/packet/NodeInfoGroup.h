//
// Created by stilgar on 08.10.17.
//

#ifndef BASYCO_NODEINFOGROUP_H
#define BASYCO_NODEINFOGROUP_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>

struct NodeInfoRequest;
struct NodeInfoResponse;

struct NodeInfoGroup : public PacketGroup {

    typedef NodeInfoRequest Request;
    typedef NodeInfoResponse Response;

};


#endif //BASYCO_NODEINFOGROUP_H
