//
// Created by stilgar on 19.11.17.
//

#ifndef BASYCO_EXECUTOR_H
#define BASYCO_EXECUTOR_H


#include <functional>
#include <memory>

class Executor {
private:
    static std::shared_ptr<Executor> executor;
public:

    static void execute(const std::function<void(void)> task) {
        executor->executeTask(task);
    }

protected:
    virtual void executeTask(std::function<void(void)> task) =0;
};

class SimpleExecutor : public Executor {
protected:
    void executeTask(std::function<void(void)> task) override;
};

class ThreadExecutor : public Executor {
protected:
    void executeTask(std::function<void(void)> task) override;
};

//@todo add ThreadPoolExecutor


#endif //BASYCO_EXECUTOR_H
