//
// Created by Krzysztof Tulidowicz on 11.02.19.
//

#ifndef BSC_CHAINEVALUATORS_H
#define BSC_CHAINEVALUATORS_H

#include <context/context/Context.h>

#include "ChainDefinitions.h"
#include "ChainContext.h"


namespace bsc {
    struct ChainEvaluators {

        template<typename T>
        static auto chainResult(const ChainIdType& id) {
            return [id](auto e, auto ... args) {
                return Context::getActiveContext()->get<ChainContext>()->getChainResult<T>(id);
            };
        }

        template<typename Stage, typename T = typename Stage::EventType>
        static auto chainResult(const Stage& stage) {
            auto id = *stage.getChainId();
            return [id](auto e, auto ... args) {
                auto activeContext = Context::getActiveContext();
                auto chainContext = activeContext->get<ChainContext>();
                auto& result = chainContext->getChainResult<T>(id);
                return result;
            };
        }

        template<typename T>
        static auto genericChainResult(const ChainIdType& id) {
            return [id](auto e, auto ... args) {
                return Context::getActiveContext()->get<ChainContext>()->getChainResult < EventWrapper < T >> (
                        id).getPayload();
            };
        }

        //@todo this method will work for all chain actions except the first one (newChain() - returns simple ChainEvent) - make and overload for that
        //this assumes that Stage::EventType is EventWrapper<T>
        template<typename Stage, typename T = typename Stage::EventType::PayloadType>
        static auto genericChainResult(const Stage& stage) {
            auto id = *stage.getChainId();
            return [id](auto e, auto ... args) {
                //@todo maybe wait if no result in context (if result is from another branch, but don't wait for too long!)
                return Context::getActiveContext()->get<ChainContext>()->getChainResult < EventWrapper < T >> (
                        id).getPayload();
            };
        }

    };
}


#endif //BSC_CHAINEVALUATORS_H
