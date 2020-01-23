//
// Created by stilgar on 21.03.19.
//

#ifndef BSC_GLOBALCHAINCONTEXT_H
#define BSC_GLOBALCHAINCONTEXT_H


#include <map>
#include "ChainDefinitions.h"
#include "ChainLock.h"

namespace bsc {
    class GlobalChainContext {
    private:
        std::map<ChainLockIdType, ChainLock> chainLocks;

    public:
        ChainLock& getChainLock(const ChainLockIdType& chainId) {
            return chainLocks[chainId];
        }

    };
}


#endif //BSC_GLOBALCHAINCONTEXT_H
