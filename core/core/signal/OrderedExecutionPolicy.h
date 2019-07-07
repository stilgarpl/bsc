//
// Created by stilgar on 29.11.18.
//

#ifndef BASYCO_ORDEREDEXECUTIONPOLICY_H
#define BASYCO_ORDEREDEXECUTIONPOLICY_H


#include "ExecutionPolicy.h"
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>


class OrderedExecutor : public Executor {
private:
    std::unique_ptr<std::thread> orderedExecutorThread;
    std::condition_variable taskReady;
    bool working = false;
    std::mutex queueLock;
    std::queue<std::pair<std::function<void(void)>, Context::Ptr>> runQueue;
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

class OrderedExecutionPolicy : public ExecutionPolicy {
private:
    std::shared_ptr<Executor> _executor = std::make_shared<OrderedExecutor>();
public:
    std::shared_ptr<Executor> executor() override;

    virtual ~OrderedExecutionPolicy() = default;
};


#endif //BASYCO_ORDEREDEXECUTIONPOLICY_H
