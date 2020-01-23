//
// Created by stilgar on 29.07.2019.
//

#ifndef BSC_SIMPLEEXECUTOR_H
#define BSC_SIMPLEEXECUTOR_H


#include "Executor.h"


namespace bsc {
    class SimpleExecutor : public bsc::Executor {
    protected:
        void execute(std::function<void(void)> task) override;

    public:

        void stop() override;
    };
}


#endif //BSC_SIMPLEEXECUTOR_H
