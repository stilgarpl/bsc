//
// Created by stilgar on 09.07.19.
//

#ifndef BASYCO_EXECUTORPOLICYTRAITS_H
#define BASYCO_EXECUTORPOLICYTRAITS_H

template<typename ExecutionPolicyType>
struct ExecutorPolicyTraits {

};


//@todo think of better way of initializing policy to executor map.
#define MAP_POLICY_TO_EXECUTOR(policy, executor) template <> struct ExecutorPolicyTraits<policy> { using ExecutorType = executor; };


#endif //BASYCO_EXECUTORPOLICYTRAITS_H
