//
// Created by stilgar on 19.11.17.
//

#ifndef BASYCO_EXECUTOR_H
#define BASYCO_EXECUTOR_H

#include <functional>

namespace bsc {
    class Executor {

    public:
        virtual void execute(std::function<void(void)> task) = 0;

        virtual void stop() = 0;

        virtual ~Executor() = default;
    };
}

#endif //BASYCO_EXECUTOR_H
