//
// Created by stilgar on 19.03.19.
//

#ifndef BASYCO_FILESYSTEMNETWORKPROCESSORS_H
#define BASYCO_FILESYSTEMNETWORKPROCESSORS_H

#include <p2p/modules/filesystem/network/packet/TransferQuery.h>

namespace bsc {
    struct FilesystemNetworkProcessors {

        static std::function<TransferQuery::Response::Ptr(TransferQuery::Request::Ptr)> queryProcessor;
    };
}


#endif //BASYCO_FILESYSTEMNETWORKPROCESSORS_H
