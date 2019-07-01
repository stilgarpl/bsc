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

void ThreadExecutor::execute(const std::function<void(void)> task) {

    Context::Ptr origContext = Context::getActiveContext();
    std::thread(
            [=]() {
                Context::setActiveContext(origContext);
                task();
            }
    ).detach();


}

//std::shared_ptr<Executor> Executor::executor = nullptr;
//std::shared_ptr<Executor> Executor::executor = std::make_shared<SimpleExecutor>();
std::shared_ptr<Executor> Executor::executor = std::make_shared<ThreadExecutor>();