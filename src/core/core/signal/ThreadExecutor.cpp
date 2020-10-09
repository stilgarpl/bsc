//
// Created by Krzysztof Tulidowicz on 29.07.2019.
//

#include <core/context/Context.h>
#include <thread>
#include "ThreadExecutor.h"


void bsc::ThreadExecutor::execute(const std::function<void(void)> task) { ContextRunner::runNewThread(task).detach(); }

void bsc::ThreadExecutor::stop() {

}