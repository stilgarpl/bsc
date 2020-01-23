//
// Created by stilgar on 07.07.18.
//

#ifndef BSC_REMOTENODECONTEXT_H
#define BSC_REMOTENODECONTEXT_H


#include "RemoteNode.h"

namespace bsc {
    class RemoteNodeContext {
    private:
        RemoteNode& _remoteNode;
    public:
        RemoteNode& getRemoteNode() {
            return _remoteNode;
        }

        explicit RemoteNodeContext(RemoteNode& _remoteNode) : _remoteNode(_remoteNode) {}
    };
}


#endif //BSC_REMOTENODECONTEXT_H
