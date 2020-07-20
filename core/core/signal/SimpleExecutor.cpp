//
// Created by Krzysztof Tulidowicz on 29.07.2019.
//

#include "SimpleExecutor.h"

void bsc::SimpleExecutor::execute(const std::function<void(void)> task) {

    task();
}

void bsc::SimpleExecutor::stop() {

}