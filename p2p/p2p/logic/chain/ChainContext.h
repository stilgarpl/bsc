//
// Created by stilgar on 11.02.19.
//

#ifndef BASYCO_CHAINCONTEXT_H
#define BASYCO_CHAINCONTEXT_H


#include "ChainDefinitions.h"
#include <map>
#include <p2p/uber/Uber.h>

class ChainContext {

private:
    ChainIdType chainId;
    template<typename ...T>
    using ChainMap = std::map<ChainIdType, T...>;
    Uber<ChainMap> chainData;

public:
    explicit ChainContext(const ChainIdType &chainId);

public:
    const ChainIdType &getChainId() const;

    template<typename EventType>
    void storeChainResult(ChainIdType id, const EventType &e) {
        chainData.get<std::shared_ptr<EventType>>()[id] = std::make_shared<EventType>(e);
    }

    template<typename EventType>
    EventType &getChainResult(ChainIdType id) {
        return *chainData.get<std::shared_ptr<EventType>>()[id];
    }

};


#endif //BASYCO_CHAINCONTEXT_H
