//
// Created by stilgar on 19.11.17.
//

#include <thread>
#include <core/context/Context.h>
#include "Executor.h"
#include "SimpleExecutor.h"
#include "ThreadExecutor.h"
#include "ThreadPoolExecutor.h"


using namespace std::chrono_literals;


//std::shared_ptr<Executor> Executor::executor = nullptr;
//std::shared_ptr<Executor> Executor::executor = std::make_shared<SimpleExecutor>();
//std::shared_ptr<Executor> Executor::executor = std::make_shared<ThreadPoolExecutor>();

Executor::~Executor() {

}
