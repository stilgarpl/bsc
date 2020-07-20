//
// Created by Krzysztof Tulidowicz on 21.03.19.
//

#ifndef BSC_CHAINLOCK_H
#define BSC_CHAINLOCK_H


#include <mutex>
#include <optional>
#include <condition_variable>
#include "ChainDefinitions.h"


namespace bsc {
    class LogicObject;

    class ChainLock {

    private:
        std::recursive_mutex mutexLock;
        bool lockedFlag = false;
        std::optional<InstanceType> instance = std::nullopt;
        std::condition_variable_any chainReady;

    public:
        [[nodiscard]] bool isLocked() const;

        [[nodiscard]] const std::optional<InstanceType>& getInstance() const;

        void lock(InstanceType newInstance);

        void unlock();

        void waitForUnlock();

        friend class LogicObject;

    private:

        decltype(mutexLock)& getMutex() {
            return mutexLock;
        }

    };
}


#endif //BSC_CHAINLOCK_H
