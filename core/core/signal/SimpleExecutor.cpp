//
// Created by stilgar on 29.07.2019.
//

#include "SimpleExecutor.h"

void SimpleExecutor::execute(const std::function<void(void)> task) {

    task();
}

void SimpleExecutor::stop() {

}