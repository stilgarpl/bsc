//
// Created by Krzysztof Tulidowicz on 22.02.2020.
//

#ifndef BSC_JOURNALFUNCMAP_H
#define BSC_JOURNALFUNCMAP_H

#include "JournalStateEntry.h"
#include "JournalTarget.h"
#include "JournalTypes.h"
#include <core/log/Logger.h>
#include <core/uber/Uber.h>
#include <functional>
#include <map>

namespace bsc {
    //@todo rename to some better name
    class JournalFuncMap {

    public:
        template<JournalTarget target>
        using Func =  std::function<void(const JournalStateEntry<target>&)> ;
        template<JournalTarget target>
        using FuncMap =  std::map<JournalMethod, Func<target>> ;

    private:
        NonTypeUber<FuncMap> funcMap;

    public:
        template<JournalMethod method, JournalTarget target>
        void setFunc(const Func<target>& func) {
            funcMap.get<target>()[method] = func;
        }

        template<JournalTarget target>
        bool execute(JournalMethod method,const JournalStateEntry<target>& state) const {
            if (funcMap.has<target>() && funcMap.get<target>().contains(method)) {
                funcMap.get<target>().at(method)(state);
                return true;
            }
            return false;
        }
    };
}// namespace bsc

#endif