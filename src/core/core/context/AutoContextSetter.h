//
// Created by Krzysztof Tulidowicz on 13.10.2020.
//

#ifndef BSC_AUTOCONTEXTSETTER_H
#define BSC_AUTOCONTEXTSETTER_H

#include "Context.h"
#include <core/auto/AutoCaller.h>
namespace bsc {

    namespace detail {
        template<typename ContextType>
        inline void registerContext() {
            Context::registerInitializer([](Context& context) { context.set<ContextType>(); });
        }
    }// namespace detail

    template<typename ContextType>
    class AutoContextSetter : public AutoCaller<ContextType, detail::registerContext<ContextType>> {
    public:
        AutoContextSetter() : AutoCaller<ContextType, detail::registerContext<ContextType>>({}) {}
    };
}// namespace bsc

#endif