//
// Created by Krzysztof Tulidowicz on 19.03.19.
//

#ifndef BSC_FILESYSTEMNETWORKPROCESSORS_H
#define BSC_FILESYSTEMNETWORKPROCESSORS_H

#include <p2p/modules/filesystem/network/packet/TransferQuery.h>

namespace bsc {
    struct FilesystemNetworkProcessors {

        static std::function<TransferQuery::Response::Ptr(TransferQuery::Request::Ptr)> queryProcessor;
    };
}


#endif //BSC_FILESYSTEMNETWORKPROCESSORS_H
