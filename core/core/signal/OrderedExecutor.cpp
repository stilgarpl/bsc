//
// Created by stilgar on 09.07.19.
//

#include "OrderedExecutor.h"

using namespace std::chrono_literals;

void OrderedExecutor::execute(std::function<void(void)> task) {
    if (orderedExecutorThread == nullptr) {
        working = true;
//        LOGGER("starting executor thread")
        orderedExecutorThread = std::make_unique<std::thread>(&OrderedExecutor::run, this);
    }
    std::lock_guard<std::mutex> lockGuard(queueLock);
    runQueue.push(std::make_pair(task, Context::getActiveContext()));

    taskReady.notify_all();
}

void OrderedExecutor::run() {
    std::unique_lock <std::mutex> g(queueLock);
    while (working) {
        while (runQueue.empty() && working) {
//            LOGGER("executor waiting")
            taskReady.wait_for(g, 1s);
        }
        while (!runQueue.empty()) {
//            LOGGER("processing task")
            auto&[task, contextPtr] = runQueue.front();

            Context::setActiveContext(contextPtr);
            task();

            runQueue.pop();
        }
    }
}

OrderedExecutor::~OrderedExecutor() {
    working = false;
    taskReady.notify_all(); // so it stops waiting
    if (orderedExecutorThread) {
        //@todo kill if not finished in few seconds?
        orderedExecutorThread->join();
    }

}

void OrderedExecutor::stop() {

}
