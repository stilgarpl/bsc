//
// Created by stilgar on 21.03.19.
//

#include "InstanceGenerator.h"

InstanceGenerator::InstanceType InstanceGenerator::nextValue() {
    std::lock_guard<std::mutex> g(valueLock);
    //@todo how to make it node-local?
    static InstanceType globalValue = 2;
    return globalValue++;
}
