//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_PROTOCOLACTIONS_H
#define BASYCO_PROTOCOLACTIONS_H


#include <p2p/modules/nodeNetworkModule/protocol/logic/events/ConnectionEvent.h>
#include "p2p/context/Context.h"


class ProtocolActions {

public:
    static void onNewConnection(const ConnectionEvent &connectionEvent);
};


#endif //BASYCO_PROTOCOLACTIONS_H
