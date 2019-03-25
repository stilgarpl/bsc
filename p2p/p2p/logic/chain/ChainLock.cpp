//
// Created by stilgar on 21.03.19.
//

#include <p2p/log/Logger.h>
#include "ChainLock.h"

using namespace std::chrono_literals;

bool ChainLock::isLocked() const {
    return _locked;
}

const std::optional<InstanceType> &ChainLock::getInstance() const {
    return _instance;
}

void ChainLock::lock(InstanceType newInstance) {
    LOGGER("LOCK " + std::to_string(newInstance))
    std::unique_lock<std::recursive_mutex> guard(_lock);
    if (_locked) {
        chainReady.wait(guard, [this, newInstance] { return !_locked || (_instance && *_instance == newInstance); });
    }
    LOGGER("LOCKING ACTUALLY")
    _locked = true;
    _instance = newInstance;
}

void ChainLock::waitForUnlock() {
    LOGGER("WAIT LOCK")
    std::unique_lock<std::recursive_mutex> guard(_lock);
    if (_locked) {
        chainReady.wait(guard, [this] { return !_locked; });
    }
}

void ChainLock::unlock() {
    LOGGER("UNLOCK")
    std::unique_lock<std::recursive_mutex> guard(_lock);
    _locked = false;
    _instance = std::nullopt;
    chainReady.notify_all();
}
