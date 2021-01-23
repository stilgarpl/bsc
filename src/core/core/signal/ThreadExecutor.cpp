//
// Created by Krzysztof Tulidowicz on 29.07.2019.
//

#include "ThreadExecutor.h"
#include <context/context/Context.h>
#include <thread>

void bsc::ThreadExecutor::execute(const std::function<void(void)> task) { ContextRunner::runNewThread(task).detach(); }

void bsc::ThreadExecutor::stop() {

}