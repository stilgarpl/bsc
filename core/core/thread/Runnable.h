//
// Created by Krzysztof Tulidowicz on 20.08.17.
//

#ifndef BSC_RUNNABLE_H
#define BSC_RUNNABLE_H


#include <thread>
#include <mutex>
#include <condition_variable>
#include <core/context/Context.h>
#include <atomic>

namespace bsc {

    class Runnable {
    private:
        //@todo switch to jthread
        std::unique_ptr<std::thread> thread;
        mutable std::mutex startMutex, stopMutex;
        std::atomic_bool stopping = false;
        std::atomic_bool started  = false;
        std::atomic_bool finished = false;

    protected:
        std::condition_variable shutdownSignal;

        void waitForStop();
        virtual void stopRequested(){};

    public:
        virtual void run() = 0;

        virtual void start() final;

        virtual void stop() final;

        virtual void join() final;

        void operator()(Context::Ptr contextPtr);

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
