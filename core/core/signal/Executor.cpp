//
// Created by stilgar on 19.11.17.
//

#include <thread>
#include <core/context/Context.h>
#include "Executor.h"


using namespace std::chrono_literals;


void SimpleExecutor::execute(const std::function<void(void)> task) {

    task();
}

void SimpleExecutor::stop() {

}

void ThreadExecutor::execute(const std::function<void(void)> task) {

    Context::Ptr origContext = Context::getActiveContext();
    std::thread(
            [=]() {
                Context::setActiveContext(origContext);
                task();
            }
    ).detach();


}

void ThreadExecutor::stop() {

}

//std::shared_ptr<Executor> Executor::executor = nullptr;
//std::shared_ptr<Executor> Executor::executor = std::make_shared<SimpleExecutor>();
std::shared_ptr<Executor> Executor::executor = std::make_shared<ThreadPoolExecutor>();

void ThreadPoolExecutor::execute(std::function<void(void)> task) {
    if (running) {
        Context::Ptr origContext = Context::getActiveContext();
        std::unique_lock<std::mutex> g(queueLock);
        taskQueue.push(std::make_pair(task, Context::getActiveContext()));
        if (getActiveWorker() < MAX_WORKER) {
            startWorker();
        }
        queueReady.notify_one();
    }

}

void ThreadPoolExecutor::startWorker() {

    std::shared_ptr<std::thread> worker = std::make_shared<std::thread>([this] {
        std::unique_lock<std::mutex> g(queueLock);
        while (running) {
            if (taskQueue.empty()) {
                //@todo 1s? configurable value?
                queueReady.wait_for(g, 1s, [&] { return !taskQueue.empty() || !running; });
            }
            if (running && !taskQueue.empty()) {
                auto[task, context_ptr] = taskQueue.front();
                taskQueue.pop();
                g.unlock();
                Context::setActiveContext(context_ptr);
                task();
                g.lock();

            }
        }
    });
    runners.push_back(worker);

}

int ThreadPoolExecutor::getActiveWorker() {
    return runners.size();
}

void ThreadPoolExecutor::stop() {
    running = false;
    queueReady.notify_all();
    for (auto &runner : runners) {
        runner->join();
    }

}

ThreadPoolExecutor::~ThreadPoolExecutor() {
    stop();

}
