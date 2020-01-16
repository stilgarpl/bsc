//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_PROTOCOLACTIONS_H
#define BASYCO_PROTOCOLACTIONS_H


#include <p2p/modules/network/protocol/logic/events/ConnectionEvent.h>
#include "core/context/Context.h"


namespace bsc {
    class ProtocolActions {

    public:
        static void onNewConnection(const ConnectionEvent& connectionEvent);
    };
}


#endif //BASYCO_PROTOCOLACTIONS_H
