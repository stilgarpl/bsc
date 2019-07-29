//
// Created by stilgar on 29.07.2019.
//

#include <core/context/Context.h>
#include <thread>
#include "ThreadExecutor.h"

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