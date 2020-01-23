//
// Created by stilgar on 09.07.19.
//

#ifndef BSC_EXECUTORPOLICYTRAITS_H
#define BSC_EXECUTORPOLICYTRAITS_H

namespace bsc {
    template<typename ExecutionPolicyType>
    struct ExecutorPolicyTraits {

    };

}

//@todo think of better way of initializing policy to executor map.
#define BSC_MAP_POLICY_TO_EXECUTOR(policy, executor) template <> struct ExecutorPolicyTraits<policy> { using ExecutorType = executor; };


#endif //BSC_EXECUTORPOLICYTRAITS_H
