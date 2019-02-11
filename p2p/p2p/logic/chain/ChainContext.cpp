//
// Created by stilgar on 11.02.19.
//

#include "ChainContext.h"

const ChainIdType &ChainContext::getChainId() const {
    return chainId;
}

ChainContext::ChainContext(const ChainIdType &chainId) : chainId(chainId) {}
