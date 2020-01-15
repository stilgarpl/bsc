//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_CLOCKSOURCE_H
#define BASYCO_CLOCKSOURCE_H

#include <chrono>
#include <logic/events/Tick.h>
#include <logic/ISource.h>

namespace bsc {
    class ClockSource : public ISource {
        typedef Tick::Clock Clock;
        typedef std::chrono::time_point<Clock> TimePoint;
        typedef Tick::Duration Duration;
    private:
        std::map<Duration, TimePoint> lastTick;
    protected:
        TimePoint getLastTick(Duration d);

        void setLastTick(Duration d, TimePoint t);

    public:
        explicit ClockSource(SourceManager& sourceManager);

        void run() override;


    };
}


#endif //BASYCO_CLOCKSOURCE_H
