//
// Created by Krzysztof Tulidowicz on 29.08.17.
//

#ifndef BSC_PROTOCOLACTIONS_H
#define BSC_PROTOCOLACTIONS_H

#include "context/context/Context.h"
#include <p2p/modules/network/protocol/logic/events/ConnectionEvent.h>

namespace bsc {
    class ProtocolActions {

    public:
        static void onNewConnection(const ConnectionEvent& connectionEvent);
    };
}


#endif //BSC_PROTOCOLACTIONS_H