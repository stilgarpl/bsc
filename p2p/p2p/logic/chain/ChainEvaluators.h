//
// Created by stilgar on 11.02.19.
//

#ifndef BASYCO_CHAINEVALUATORS_H
#define BASYCO_CHAINEVALUATORS_H


#include <p2p/context/Context.h>
#include <p2p/logic/events/EventWrapper.h>
#include "ChainDefinitions.h"
#include "ChainContext.h"

struct ChainEvaluators {

    template<typename T>
    static auto chainResult(const ChainIdType &id) {
        return [id](auto e, auto ... args) {
            return Context::getActiveContext()->get<ChainContext>()->getChainResult<T>(id);
        };
    }

    template<typename T>
    static auto genericChainResult(const ChainIdType &id) {
        return [id](auto e, auto ... args) {
            return Context::getActiveContext()->get<ChainContext>()->getChainResult<EventWrapper<T>>(id).getPayload();
        };
    }

    template<typename T, typename Stage>
    static auto genericChainResult(const Stage &stage) {
        auto id = stage.getChainId();
        return [id](auto e, auto ... args) {
            return Context::getActiveContext()->get<ChainContext>()->getChainResult<EventWrapper<T>>(id).getPayload();
        };
    }

};


#endif //BASYCO_CHAINEVALUATORS_H
