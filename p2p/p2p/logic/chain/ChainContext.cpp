//
// Created by stilgar on 11.02.19.
//

#include <p2p/log/Logger.h>
#include "ChainContext.h"
#include <p2p/context/Context.h>
#include <p2p/node/context/NodeContext.h>


const ChainIdType &ChainContext::getChainId() const {
    return chainId;
}

ChainContext::ChainContext(const ChainIdType &chainId) : chainId(chainId) {

    NODECONTEXTLOGGER("chain context construct " + chainId)
}

ChainContext::~ChainContext() {
    NODECONTEXTLOGGER("chain context destruct " + chainId)
}
