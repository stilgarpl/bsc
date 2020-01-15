//
// Created by stilgar on 29.07.2019.
//

#ifndef BASYCO_THREADEXECUTOR_H
#define BASYCO_THREADEXECUTOR_H


#include "Executor.h"

namespace bsc {
    class ThreadExecutor : public Executor {
    protected:
        void execute(std::function<void(void)> task) override;

    public:


        void stop() override;
    };
}


#endif //BASYCO_THREADEXECUTOR_H
