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
    //@todo remove static execute, replace with virtual execute
public:

    static void executeDefault(const std::function<void(void)> &task) {
        executor->execute(task);
    }

    static std::shared_ptr<Executor> &getDefaultExecutor() {
        return executor;
    }

public:
    virtual void execute(std::function<void(void)> task) = 0;
};

class SimpleExecutor : public Executor {
protected:
    void execute(std::function<void(void)> task) override;
};

class ThreadExecutor : public Executor {
protected:
    void execute(std::function<void(void)> task) override;
};

//@todo add ThreadPoolExecutor


#endif //BASYCO_EXECUTOR_H
