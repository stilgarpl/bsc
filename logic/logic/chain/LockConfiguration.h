//
// Created by stilgar on 4/18/19.
//

#ifndef BSC_LOCKCONFIGURATION_H
#define BSC_LOCKCONFIGURATION_H


#include "ChainDefinitions.h"
#include <functional>

namespace bsc {
    class LockConfiguration {
    private:
        inline static const std::string groupPrefix = ">GROUP>";
        inline static const std::string chainPrefix = ">CHAIN>";
        inline static const std::string evalPrefix = ">EVAL>";

    public:


        static auto group(const ChainIdType& groupId) {
            return [groupPrefix = groupPrefix, groupId](const ChainIdType& chainId, auto& event) {
                //conversion from group id to lock id
                return groupPrefix + groupId;
            };
        }

        static auto chain() {
            return [chainPrefix = chainPrefix](const ChainIdType& chainId, auto& event) {
                //conversion from chain id to lock id
                return chainPrefix + chainId;
            };
        }

        template<typename Func>
        static auto eval(Func func) {
            return [evalPrefix = evalPrefix, func](const ChainIdType& chainId, auto& event) {
                //@todo I'm quite sure this can be implemented better
                //conversion from evaluated id to lock id
                return evalPrefix + func(event);
            };
        }

    };
}


#endif //BSC_LOCKCONFIGURATION_H
