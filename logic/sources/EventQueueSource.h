//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_EVENTQUEUESOURCE_H
#define BASYCO_EVENTQUEUESOURCE_H


#include <queue>
#include "../ISource.h"
#include "../SourceManager.h"
#include "../../log/Logger.h"
#include "../../network/protocol/context/NodeContext.h"

class NullClass {
};

template<typename EventType_, typename FriendClass=NullClass>
class EventQueueSource : public ISource {
    friend FriendClass;
protected:
    typedef EventType_ EventType;
public:
    typedef std::shared_ptr<EventType_> EventTypePtr;
private:
    ///@todo thread safety
    std::queue<EventTypePtr> eventQueue;
protected:
    void queueEvent(EventTypePtr event) {
        eventQueue.push(event);
    }

    template<typename ... Args>
    EventTypePtr newEvent(Args... args) {
        return std::make_shared<EventType_>(args...);
    }
public:
    void work() override {

        while (!eventQueue.empty()) {
            //  NODECONTEXTLOGGER("Processing event");
            auto &i = eventQueue.front();
            this->event(*i);
            eventQueue.pop();
        }

    }

    void registerProviders(SourceManager *manager) override {
        manager->registerProvider<EventType_>();
    }


    EventQueueSource() = default;


};


#endif //BASYCO_EVENTQUEUESOURCE_H
