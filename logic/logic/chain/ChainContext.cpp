//
// Created by stilgar on 11.02.19.
//

#include <core/log/Logger.h>
#include "ChainContext.h"
#include <core/context/Context.h>


const ChainIdType &ChainContext::getChainId() const {
    return chainId;
}

ChainContext::ChainContext(const ChainIdType &chainId) : chainId(chainId) {

//    LOGGER("chain context construct " + chainId)
}

ChainContext::~ChainContext() {
//    LOGGER("chain context destruct " + chainId)
}
