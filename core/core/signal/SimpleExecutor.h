//
// Created by stilgar on 29.07.2019.
//

#ifndef BASYCO_SIMPLEEXECUTOR_H
#define BASYCO_SIMPLEEXECUTOR_H

#include "Executor.h"


class SimpleExecutor : public Executor {
protected:
    void execute(std::function<void(void)> task) override;

public:

    void stop() override;
};


#endif //BASYCO_SIMPLEEXECUTOR_H
