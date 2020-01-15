//
// Created by stilgar on 21.03.19.
//

#ifndef BASYCO_GLOBALCHAINCONTEXT_H
#define BASYCO_GLOBALCHAINCONTEXT_H


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


#endif //BASYCO_GLOBALCHAINCONTEXT_H
