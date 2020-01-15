//
// Created by stilgar on 21.03.19.
//

#include <core/log/Logger.h>
#include "ChainLock.h"


using namespace std::chrono_literals;

bool bsc::ChainLock::isLocked() const {
    return lockedFlag;
}

const std::optional<bsc::InstanceType>& bsc::ChainLock::getInstance() const {
    return instance;
}

void bsc::ChainLock::lock(InstanceType newInstance) {
    LOGGER("LOCK " + std::to_string(newInstance))
    std::unique_lock<std::recursive_mutex> guard(mutexLock);
    if (lockedFlag) {
        LOGGER("waiting for lock")
        chainReady.wait(guard, [this, newInstance] { return !lockedFlag || (instance && *instance == newInstance); });
    }
    LOGGER("LOCKING ACTUALLY")
    lockedFlag = true;
    instance = newInstance;
}

void bsc::ChainLock::waitForUnlock() {
    LOGGER("WAIT LOCK")
    std::unique_lock<std::recursive_mutex> guard(mutexLock);
    if (lockedFlag) {
        chainReady.wait(guard, [this] { return !lockedFlag; });
    }
}

void bsc::ChainLock::unlock() {
    LOGGER("UNLOCK")
    std::unique_lock<std::recursive_mutex> guard(mutexLock);
    lockedFlag = false;
    instance = std::nullopt;
    chainReady.notify_all();
}
