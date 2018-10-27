//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_SOURCE_H
#define BASYCO_SOURCE_H


#include "p2p/signal/Signal.h"
#include "p2p/context/Context.h"
#include "p2p/uber/Uber.h"
#include "p2p/log/Logger.h"
#include "SourceManager.h"


class ISource {
private:
protected:
    ///@todo protected or maybe private?
    SourceManager &sourceManager;
public:
    explicit ISource(SourceManager &sourceManager);

protected:


    template<typename T, typename... Args>
    void event(const T &event, Args... args) {

        sourceManager.event(event, args...);

//        //@todo pass to executor
//        Context::setActiveContext(&context);
//        /*int a =*/ this->getSignal<T, Args...>(event.getEventId()).signal(event, args...);
//        /*int b =*/ this->getSignal<T, Args...>().signal(event, args...);
//        // LOGGER("event : a "+ std::to_string(a)+ " b:" + std::to_string(b));
    }


public:






    virtual void work() =0;

    void setContext(Context &context) {};

};


#endif //BASYCO_SOURCE_H
