//
// Created by stilgar on 29.07.2019.
//

#include <core/context/Context.h>
#include <thread>
#include "ThreadExecutor.h"


void bsc::ThreadExecutor::execute(const std::function<void(void)> task) {

    bsc::Context::Ptr origContext = bsc::Context::getActiveContext();
    std::thread(
            [=]() {
                bsc::Context::setActiveContext(origContext);
                task();
            }
    ).detach();


}

void bsc::ThreadExecutor::stop() {

}