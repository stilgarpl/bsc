//
// Created by stilgar on 21.03.19.
//

#include <core/log/Logger.h>
#include "ChainLock.h"

using namespace std::chrono_literals;

bool ChainLock::isLocked() const {
    return lockedFlag;
}

const std::optional<InstanceType> &ChainLock::getInstance() const {
    return instance;
}

void ChainLock::lock(InstanceType newInstance) {
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

void ChainLock::waitForUnlock() {
    LOGGER("WAIT LOCK")
    std::unique_lock<std::recursive_mutex> guard(mutexLock);
    if (lockedFlag) {
        chainReady.wait(guard, [this] { return !lockedFlag; });
    }
}

void ChainLock::unlock() {
    LOGGER("UNLOCK")
    std::unique_lock<std::recursive_mutex> guard(mutexLock);
    lockedFlag = false;
    instance = std::nullopt;
    chainReady.notify_all();
}
