//
// Created by stilgar on 03.02.19.
//

#ifndef BSC_COMMONACTIONS_H
#define BSC_COMMONACTIONS_H

#include <core/context/Context.h>
#include "../common/ForeachContext.h"


namespace bsc {
    struct CommonActions {

        constexpr static auto passthroughTransformer = [](const auto& e) { return e; };

        //    every(1s).fireC(foreachAction(broadcast),bulkEval({1,2,3}),bulkEval(getRepoIds()));
        template<typename Func, typename Container>
        static constexpr auto foreachAction(Func func, Container& container) {
            //don't even think about making &container. it won't work.
            return [container, func](const auto& ... param) {
                for (const auto& it : container) {
                    Context::getActiveContext()->get < ForeachContext < std::decay_t<decltype(it)>>>().setValue(it);
                    func(param...);
                }
            };
        }

        template<typename Func, typename ContainerGetter, typename Transformer=decltype(passthroughTransformer)>
        static constexpr auto foreachActionGetter(Func func, ContainerGetter containerGetter,
                                                  Transformer transformer = passthroughTransformer) {
            return [containerGetter, func, transformer](const auto& ... param) {
                for (const auto& it : containerGetter()) {
                    const auto& transformIt = transformer(it);
                    Context::getActiveContext()->get < ForeachContext <
                    std::decay_t<decltype(transformIt)>>>()->setValue(
                            transformIt);
                    func(param...);
                }
            };
        }
    };
}


#endif //BSC_COMMONACTIONS_H
