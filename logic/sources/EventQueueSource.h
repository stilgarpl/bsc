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
    std::mutex queueLock;
    std::queue<EventTypePtr> eventQueue;
protected:
    void queueEvent(EventTypePtr event) {
        std::lock_guard<std::mutex> g(queueLock);
        eventQueue.push(event);
    }

    template<typename ... Args>
    EventTypePtr newEvent(Args... args) {
        return std::make_shared<EventType_>(args...);
    }
public:
    void work() override {
        //std::lock_guard<std::mutex> g(queueLock);
        queueLock.lock();
        while (!eventQueue.empty()) {
            //  NODECONTEXTLOGGER("Processing event");
            ///@todo better locking and unlocking of mutex
            auto &i = eventQueue.front();

            queueLock.unlock();
            if (i != nullptr) {
                this->event(*i);
            } else {
                LOGGER("BAD POINTER");
            }
            queueLock.lock();
            eventQueue.pop();


        }
        queueLock.unlock();

    }

    void registerProviders(SourceManager *manager) override {
        manager->registerProvider<EventType_>();
    }


    EventQueueSource() = default;


};


#endif //BASYCO_EVENTQUEUESOURCE_H
