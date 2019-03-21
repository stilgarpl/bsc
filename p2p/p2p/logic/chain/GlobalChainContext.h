//
// Created by stilgar on 21.03.19.
//

#ifndef BASYCO_GLOBALCHAINCONTEXT_H
#define BASYCO_GLOBALCHAINCONTEXT_H


#include <map>
#include "ChainDefinitions.h"
#include "ChainLock.h"

class GlobalChainContext {
private:
    std::map<ChainIdType, ChainLock> chainLocks;

public:
    ChainLock &getChainLock(const ChainIdType &chainId) {
        return chainLocks[chainId];
    }

};


#endif //BASYCO_GLOBALCHAINCONTEXT_H
