

//
// Created by Krzysztof Tulidowicz on 4/18/19.
//

#include <utility>
#include "ChainGroup.h"

bsc::ChainGroup::ChainGroup(ChainIdType chainIdType) : chainGroupId(std::move(chainIdType)) {}

const bsc::ChainIdType& bsc::ChainGroup::getChainGroupId() const {
    return chainGroupId;
}

