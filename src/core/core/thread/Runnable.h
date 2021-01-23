//
// Created by Krzysztof Tulidowicz on 20.08.17.
//

#ifndef BSC_RUNNABLE_H
#define BSC_RUNNABLE_H

#include <atomic>
#include <condition_variable>
#include <context/context/Context.h>
#include <mutex>
#include <thread>

namespace bsc {

    class Runnable {
    private:
        std::jthread thread;
        mutable std::mutex startMutex, stopMutex;
        std::atomic_bool threadStarted = false;
        std::atomic_bool stopping      = false;
        std::atomic_bool started  = false;
        std::atomic_bool finished = false;

    protected:
        //@todo investigate if this is still needed now that we have jthread. migrate to stop_token.
        std::condition_variable shutdownSignal;

        void waitForStop();
        virtual void stopRequested(){};

    public:
        virtual void run() = 0;

        virtual void start() final;

        virtual void stop() final;

        virtual void join() final;

        void operator()();

        virtual ~Runnable();

        bool isStopping() const;

        bool isStarted() const;

        bool isFinished() const;


        //actions:
        virtual void onStop() {};

        virtual void onStart() {};
    };
}


#endif //BSC_RUNNABLE_H
