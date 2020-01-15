//
// Created by stilgar on 29.07.2019.
//

#include <core/context/Context.h>
#include "ThreadPoolExecutor.h"


using namespace std::chrono_literals;

void bsc::ThreadPoolExecutor::execute(std::function<void(void)> task) {
    if (running.load()) {
        bsc::Context::Ptr origContext = bsc::Context::getActiveContext();
        std::unique_lock<std::mutex> g(queueLock);
        taskQueue.push(std::make_pair(task, bsc::Context::getActiveContext()));
        if (getActiveWorkerCount() < maxWorker) {
            startWorker();
        }
        queueReady.notify_one();
    }

}

void bsc::ThreadPoolExecutor::startWorker() {

    std::shared_ptr<std::thread> worker = std::make_shared<std::thread>([this] {
        std::unique_lock<std::mutex> g(queueLock);
        while (running) {
            if (taskQueue.empty()) {
                //@todo 1s? configurable value?
                queueReady.wait_for(g, 1s, [&] { return !taskQueue.empty() || !running; });
            }
            if (running && !taskQueue.empty()) {
                auto[task, contextPtr] = taskQueue.front();
                taskQueue.pop();
                g.unlock();
                bsc::Context::setActiveContext(contextPtr);
                task();
                g.lock();

            }
        }
    });
    runners.push_back(worker);

}

auto bsc::ThreadPoolExecutor::getActiveWorkerCount() -> decltype(runners.size()) {
    return runners.size();
}

void bsc::ThreadPoolExecutor::stop() {
    running.store(false);
    queueReady.notify_all();
    for (auto& runner : runners) {
        runner->join();
    }

}

bsc::ThreadPoolExecutor::~ThreadPoolExecutor() {
    stop();

}

