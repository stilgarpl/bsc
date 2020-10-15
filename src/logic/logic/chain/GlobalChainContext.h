//
// Created by Krzysztof Tulidowicz on 21.03.19.
//

#ifndef BSC_GLOBALCHAINCONTEXT_H
#define BSC_GLOBALCHAINCONTEXT_H

#include "ChainDefinitions.h"
#include "ChainLock.h"
#include <core/context/AutoContextSetter.h>
#include <map>

namespace bsc {
    class GlobalChainContext : public AutoContextSetter<GlobalChainContext> {
    private:
        std::map<ChainLockIdType, ChainLock> chainLocks;

    public:
        ChainLock& getChainLock(const ChainLockIdType& chainId) { return chainLocks[chainId]; }
    };
}


#endif //BSC_GLOBALCHAINCONTEXT_H
