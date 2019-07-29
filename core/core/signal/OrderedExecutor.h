//
// Created by stilgar on 09.07.19.
//

#ifndef BASYCO_ORDEREDEXECUTOR_H
#define BASYCO_ORDEREDEXECUTOR_H

#include <memory>
#include <condition_variable>
#include <queue>
#include <core/context/Context.h>
#include "Executor.h"
#include <thread>

class OrderedExecutor : public Executor {
private:
    std::unique_ptr <std::thread> orderedExecutorThread;
    std::condition_variable taskReady;
    bool working = false;
    std::mutex queueLock;
    std::queue <std::pair<std::function < void(void)>, Context::Ptr>> runQueue;
public:
    void execute(std::function<void(void)> task) override;

protected:
public:
    virtual ~OrderedExecutor();

protected:
    void run();

public:
    void stop() override;

};


#endif //BASYCO_ORDEREDEXECUTOR_H
