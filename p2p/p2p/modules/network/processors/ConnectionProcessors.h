//
// Created by stilgar on 10.12.2019.
//

#ifndef BSC_CONNECTIONPROCESSORS_H
#define BSC_CONNECTIONPROCESSORS_H


#include <p2p/modules/network/protocol/packet/ConnectionControl.h>

namespace bsc {
    struct ConnectionProcessors {

        static ConnectionControl::Response::Ptr processConnectionControl(ConnectionControl::Request::Ptr request);

    };
}


#endif //BSC_CONNECTIONPROCESSORS_H
