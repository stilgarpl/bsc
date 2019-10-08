//
// Created by stilgar on 29.07.2019.
//

#ifndef BASYCO_THREADPOOLEXECUTOR_H
#define BASYCO_THREADPOOLEXECUTOR_H

#include "Executor.h"
#include <queue>
#include <condition_variable>
#include <list>
#include <thread>
#include <atomic>


class ThreadPoolExecutor : public Executor {
private:
    std::queue<std::pair<std::function<void(void)>, Context::Ptr>> taskQueue;
    std::mutex queueLock;
    std::condition_variable queueReady;
    std::list<std::shared_ptr<std::thread>> runners;
    std::atomic<bool> running = true;
    unsigned MAX_WORKER = 5;

public:
    void execute(std::function<void(void)> task) override;

protected:
    void startWorker();

    auto getActiveWorkerCount() -> decltype(runners.size());

public:
    void stop() override;

    ~ThreadPoolExecutor() override;

};

#endif //BASYCO_THREADPOOLEXECUTOR_H
