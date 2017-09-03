//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_PROTOCOLACTIONS_H
#define BASYCO_PROTOCOLACTIONS_H


#include "../../../context/Context.h"
#include "../../protocol/logic/events/ConnectionEvent.h"

class ProtocolActions {

public:
    static void onNewConnection(Context &, const ConnectionEvent &connectionEvent);
};


#endif //BASYCO_PROTOCOLACTIONS_H
