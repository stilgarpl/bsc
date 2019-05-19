//
// Created by stilgar on 4/18/19.
//

#ifndef BASYCO_LOCKCONFIGURATION_H
#define BASYCO_LOCKCONFIGURATION_H


#include "ChainDefinitions.h"
#include <functional>

class LockConfiguration {
private:
    inline static const std::string GROUP_PREFIX = ">GROUP>";
    inline static const std::string CHAIN_PREFIX = ">CHAIN>";
    inline static const std::string EVAL_PREFIX = ">EVAL>";

public:


    static auto group(const ChainIdType &groupId) {
        return [GROUP_PREFIX = GROUP_PREFIX, groupId](const ChainIdType &chainId, auto &event) {
            //conversion from group id to lock id
            return GROUP_PREFIX + groupId;
        };
    }

    static auto chain() {
        return [CHAIN_PREFIX = CHAIN_PREFIX](const ChainIdType &chainId, auto &event) {
            //conversion from chain id to lock id
            return CHAIN_PREFIX + chainId;
        };
    }

    template<typename Func>
    static auto eval(Func func) {
        return [EVAL_PREFIX = EVAL_PREFIX, func](const ChainIdType &chainId, auto &event) {
            //@todo I'm quite sure this can be implemented better
            //conversion from evaluated id to lock id
            return EVAL_PREFIX + func(event);
        };
    }

};


#endif //BASYCO_LOCKCONFIGURATION_H