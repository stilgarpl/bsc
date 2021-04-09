//
// Created by Krzysztof Tulidowicz on 21.03.19.
//

#include "ChainLock.h"
#include <log/log/Logger.h>

using namespace std::chrono_literals;

bool bsc::ChainLock::isLocked() const {
    return lockedFlag;
}

const std::optional<bsc::InstanceType>& bsc::ChainLock::getInstance() const {
    return instance;
}

void bsc::ChainLock::lock(InstanceType newInstance) {
    logger.debug("LOCK " + std::to_string(newInstance));
    std::unique_lock<std::recursive_mutex> guard(mutexLock);
    if (lockedFlag) {
        logger.debug("waiting for lock");
        chainReady.wait(guard, [this, newInstance] { return !lockedFlag || (instance && *instance == newInstance); });
    }
    logger.debug("LOCKING ACTUALLY");
    lockedFlag = true;
    instance = newInstance;
}

void bsc::ChainLock::waitForUnlock() {
    logger.debug("WAIT LOCK");
    std::unique_lock<std::recursive_mutex> guard(mutexLock);
    if (lockedFlag) {
        chainReady.wait(guard, [this] { return !lockedFlag; });
    }
}

void bsc::ChainLock::unlock() {
    logger.debug("UNLOCK");
    std::unique_lock<std::recursive_mutex> guard(mutexLock);
    lockedFlag = false;
    instance = std::nullopt;
    chainReady.notify_all();
}
