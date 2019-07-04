//
// Created by stilgar on 08.10.17.
//

#ifndef BASYCO_NETWORKINFOGROUP_H
#define BASYCO_NETWORKINFOGROUP_H

#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>

class NetworkInfoRequest;

class NetworkInfoResponse;

struct NetworkInfoGroup : public PacketGroup {

    typedef NetworkInfoRequest Request;
    typedef NetworkInfoResponse Response;

};


#endif //BASYCO_NETWORKINFOGROUP_H