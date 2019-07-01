//
// Created by stilgar on 21.03.19.
//

#ifndef BASYCO_CHAINLOCK_H
#define BASYCO_CHAINLOCK_H


#include <mutex>
#include <optional>
#include <condition_variable>
#include "ChainDefinitions.h"

class LogicObject;


class ChainLock {

private:
    std::recursive_mutex _lock;
    bool _locked = false;
    std::optional<InstanceType> _instance = std::nullopt;
    std::condition_variable_any chainReady;

public:
    bool isLocked() const;

    const std::optional<InstanceType> &getInstance() const;

    void lock(InstanceType newInstance);

    void unlock();

    void waitForUnlock();

    friend class LogicObject;

private:

    decltype(_lock)& getMutex() {
        return _lock;
    }

};


#endif //BASYCO_CHAINLOCK_H
