//
// Created by Krzysztof Tulidowicz on 23.08.17.
//

#ifndef BSC_TICK_H
#define BSC_TICK_H


#include <chrono>
#include "../IEvent.h"

namespace bsc {
    /**
         * tick every set amount of time
         */
    class Tick : public bsc::IEvent<std::chrono::milliseconds> {
    public:
        typedef std::chrono::steady_clock Clock;
        typedef std::chrono::milliseconds Duration;

    private:
        std::chrono::time_point<Clock> now;

    public:
        [[nodiscard]] const std::chrono::time_point<Clock>& getNow() const;

        void setNow(const std::chrono::time_point<Clock>& now);

        Tick();
    };
}


#endif //BSC_TICK_H
