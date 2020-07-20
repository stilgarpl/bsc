//
// Created by Krzysztof Tulidowicz on 11.02.19.
//

#include "ChainContext.h"


const bsc::ChainIdType& bsc::ChainContext::getChainId() const {
    return chainId;
}

bsc::ChainContext::ChainContext(const ChainIdType& chainId) : chainId(chainId) {

//    LOGGER("chain context construct " + chainId)
}

bsc::ChainContext::~ChainContext() {
//    LOGGER("chain context destruct " + chainId)
}
