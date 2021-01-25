//
// Created by Krzysztof Tulidowicz on 29.07.2019.
//

#ifndef BSC_THREADPOOLEXECUTOR_H
#define BSC_THREADPOOLEXECUTOR_H

#include "Executor.h"
#include <atomic>
#include <condition_variable>
#include <context/context/Context.h>
#include <list>
#include <queue>
#include <thread>

namespace bsc {
    class ThreadPoolExecutor : public Executor {
    private:
        std::queue<std::pair<std::function<void(void)>, Context::Ptr>> taskQueue;
        std::mutex queueLock;
        std::condition_variable queueReady;
        std::list<std::shared_ptr<std::thread>> runners;
        std::atomic<bool> running = true;
        unsigned maxWorker = 500;

    public:
        void execute(std::function<void(void)> task) override;

    protected:
        void startWorker();

        auto getActiveWorkerCount() -> decltype(runners.size());

    public:
        void stop() override;

        ~ThreadPoolExecutor() override;

    };
}

#endif //BSC_THREADPOOLEXECUTOR_H
