//
// Created by stilgar on 4/18/19.
//

#ifndef BSC_CHAINGROUP_H
#define BSC_CHAINGROUP_H


#include "ChainDefinitions.h"

namespace bsc {
    class ChainGroup {

    private:
        ChainIdType chainGroupId;

    public:
        ChainGroup(ChainIdType chainIdType);

        const ChainIdType& getChainGroupId() const;

    };
}


#endif //BSC_CHAINGROUP_H
