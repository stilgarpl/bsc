//
// Created by stilgar on 11.02.19.
//

#ifndef BASYCO_CHAINCONTEXT_H
#define BASYCO_CHAINCONTEXT_H


#include "ChainDefinitions.h"
#include "InstanceGenerator.h"
#include <map>
#include <core/uber/Uber.h>

class ChainContext {

private:
    ChainIdType chainId;
    template<typename ...T>
    using ChainMap = std::map<ChainIdType, T...>;
    Uber<ChainMap> chainData;
    InstanceGenerator _instanceGenerator;

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
        auto result = chainData.get<std::shared_ptr<EventType>>()[id];
        return *result;
    }

    virtual ~ChainContext();

    //@todo maybe move it somewhere from context? or not.
    InstanceGenerator &instanceGenerator() {
        return _instanceGenerator;
    }

};


#endif //BASYCO_CHAINCONTEXT_H
