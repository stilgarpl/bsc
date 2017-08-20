//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_RUNNABLE_H
#define BASYCO_RUNNABLE_H


#include <thread>

class Runnable {
private:
    std::unique_ptr<std::thread> thread;
    bool stopping = false;
public:

    virtual void run() = 0;

    virtual void start() final;

    virtual void stop() final;

    void operator()();

    virtual ~Runnable();

    bool isStopping() const;
};


#endif //BASYCO_RUNNABLE_H
