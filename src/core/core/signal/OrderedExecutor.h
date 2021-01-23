//
// Created by Krzysztof Tulidowicz on 09.07.19.
//

#ifndef BSC_ORDEREDEXECUTOR_H
#define BSC_ORDEREDEXECUTOR_H

#include <condition_variable>
#include <context/context/Context.h>
#include <memory>
#include <queue>

#include <thread>
#include <atomic>
#include "Executor.h"

namespace bsc {
    class OrderedExecutor : public Executor {
    private:
        std::unique_ptr<std::thread> orderedExecutorThread;
        std::condition_variable taskReady;
        std::atomic<bool> working = false;
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
}


#endif //BSC_ORDEREDEXECUTOR_H
