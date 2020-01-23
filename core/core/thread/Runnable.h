//
// Created by stilgar on 20.08.17.
//

#ifndef BSC_RUNNABLE_H
#define BSC_RUNNABLE_H


#include <thread>
#include <mutex>
#include <condition_variable>
#include <core/context/Context.h>
#include <atomic>


using namespace std::chrono_literals;

namespace bsc {
    class Runnable {
    private:
        std::unique_ptr<std::thread> thread;
        mutable std::mutex startMutex, stopMutex;
        std::atomic_bool stopping = false;
        std::atomic_bool finished = false;
    protected:
        std::condition_variable shutdownSignal;

        void waitForStop();

    public:

        virtual void run() = 0;

        virtual void start() final;

        virtual void stop() final;

        virtual void join() final;

        void operator()(Context::Ptr contextPtr);

        virtual ~Runnable();

        bool isStopping() const;


        //actions:
        virtual void onStop() {};

        virtual void onStart() {};
    };
}


#endif //BSC_RUNNABLE_H
