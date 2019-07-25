//
// Created by stilgar on 19.11.17.
//

#ifndef BASYCO_EXECUTOR_H
#define BASYCO_EXECUTOR_H


#include <functional>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <list>
#include <thread>
#include <core/context/Context.h>

class Executor {
private:
    static std::shared_ptr<Executor> executor;
public:

    static std::shared_ptr<Executor> &getDefaultExecutor() {
        return executor;
    }

public:
    virtual void execute(std::function<void(void)> task) = 0;
    virtual ~Executor() = default;

    virtual void stop() = 0;
};

class SimpleExecutor : public Executor {
protected:
    void execute(std::function<void(void)> task) override;
public:
    ~SimpleExecutor() override = default;

    void stop() override;
};

class ThreadExecutor : public Executor {
protected:
    void execute(std::function<void(void)> task) override;
public:
    ~ThreadExecutor() override = default;

    void stop() override;
};


class ThreadPoolExecutor : public Executor {
private:
    std::queue<std::pair<std::function<void(void)>, Context::Ptr>> taskQueue;
    std::mutex queueLock;
    std::condition_variable queueReady;
    std::list<std::shared_ptr<std::thread>> runners;
    bool running = true;
    unsigned MAX_WORKER = 5;

public:
    void execute(std::function<void(void)> task) override;

protected:
    void startWorker();

    auto getActiveWorkerCount() -> decltype(runners.size());

public:
    void stop() override;

    virtual ~ThreadPoolExecutor();

};

//@todo add ThreadPoolExecutor


#endif //BASYCO_EXECUTOR_H
