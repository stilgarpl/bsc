//
// Created by stilgar on 22.02.2020.
//

#ifndef BSC_JOURNALFUNCMAP_H
#define BSC_JOURNALFUNCMAP_H
#include "JournalState.h"
#include "JournalTypes.h"
#include <functional>
#include <map>

namespace bsc {
    //@todo rename to some better name
    class JournalFuncMap {

    public:
        typedef std::function<void(const JournalStateData&)> Func;
        typedef std::map<std::pair<JournalMethod, JournalTarget>, Func> FuncMap;

    private:
        FuncMap funcMap;

    public:
        void setFunc(JournalMethod method, JournalTarget target, const Func& func) {
            funcMap[std::make_pair(method, target)] = func;
        }

        bool execute(const JournalStateData& state) {
            auto id = std::make_pair(state.getMethod(), state.getTarget());
            if (funcMap.contains(id)) {
                funcMap[id](state);
                return true;
            } else {
                return false;
            }
        }
    };
}// namespace bsc

#endif