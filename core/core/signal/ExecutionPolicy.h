//
// Created by stilgar on 29.11.18.
//

#ifndef BASYCO_EXECUTIONPOLICY_H
#define BASYCO_EXECUTIONPOLICY_H


#include "Executor.h"

class ExecutionPolicy {
public:
    virtual std::shared_ptr<Executor> executor() = 0;
};


#endif //BASYCO_EXECUTIONPOLICY_H
