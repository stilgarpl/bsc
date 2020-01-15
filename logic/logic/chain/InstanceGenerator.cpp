//
// Created by stilgar on 21.03.19.
//

#include "InstanceGenerator.h"
#include <atomic>

bsc::InstanceGenerator::InstanceType bsc::InstanceGenerator::nextValue() {
    std::lock_guard<std::mutex> g(valueLock);
    //@todo how to make it node-local?
    static std::atomic<InstanceType> globalValue = 2;
    return globalValue.fetch_add(1);
}
