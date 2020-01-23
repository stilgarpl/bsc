//
// Created by stilgar on 29.07.2019.
//

#ifndef BSC_THREADEXECUTOR_H
#define BSC_THREADEXECUTOR_H


#include "Executor.h"

namespace bsc {
    class ThreadExecutor : public Executor {
    protected:
        void execute(std::function<void(void)> task) override;

    public:


        void stop() override;
    };
}


#endif //BSC_THREADEXECUTOR_H
