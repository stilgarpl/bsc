

//
// Created by stilgar on 4/18/19.
//

#include <utility>
#include "ChainGroup.h"

ChainGroup::ChainGroup(ChainIdType chainIdType) : chainGroupId(std::move(chainIdType)) {}

const ChainIdType &ChainGroup::getChainGroupId() const {
    return chainGroupId;
}

