//
// Created by stilgar on 19.11.17.
//

#include <thread>
#include <p2p/context/Context.h>
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
    //delay to prevent stack smashing, the error is probably somewhere else
    // std::this_thread::sleep_for(1ms);
//    if (t.joinable()) {
//        t.detach();
//    }

}

//std::shared_ptr<Executor> Executor::executor = nullptr;
//std::shared_ptr<Executor> Executor::executor = std::make_shared<SimpleExecutor>();
std::shared_ptr<Executor> Executor::executor = std::make_shared<ThreadExecutor>();