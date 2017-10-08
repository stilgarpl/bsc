//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_PROTOCOLACTIONS_H
#define BASYCO_PROTOCOLACTIONS_H


#include <network/protocol/logic/events/ConnectionEvent.h>
#include "context/Context.h"


class ProtocolActions {

public:
    static void onNewConnection(const ConnectionEvent &connectionEvent);
};


#endif //BASYCO_PROTOCOLACTIONS_H
