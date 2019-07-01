//
// Created by stilgar on 4/18/19.
//

#ifndef BASYCO_CHAINGROUP_H
#define BASYCO_CHAINGROUP_H


#include "ChainDefinitions.h"

class ChainGroup {

private:
    ChainIdType chainGroupId;

public:
    ChainGroup(ChainIdType chainIdType);

    const ChainIdType &getChainGroupId() const;

};


#endif //BASYCO_CHAINGROUP_H
