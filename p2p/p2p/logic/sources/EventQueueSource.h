//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_EVENTQUEUESOURCE_H
#define BASYCO_EVENTQUEUESOURCE_H


#include <queue>
#include <p2p/logic/ISource.h>
#include "p2p/log/Logger.h"
#include "p2p/network/protocol/context/NodeContext.h"


class NullClass {
};

template<typename EventType_, typename FriendClass=NullClass>
class EventQueueSource : public ISource {
    friend FriendClass;
protected:
    typedef EventType_ EventType;
    typedef typename EventType_::IdType EventIdType;
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
        auto ret = std::make_shared<EventType_>(args...);
        ///@todo think about setup method and make it better and cleaner? I'm not fond of the interface
//        LOGGER(typeid(EventType_).name())
        EventType::setup(ret.get());
        ///@todo shouldn't this be in the EventType (IEvent) constructor?
        setupOrigin<typename EventType::OriginType>::setup(ret->origin());
        return ret;
    }
public:
    explicit EventQueueSource(SourceManager &sourceManager) : ISource(sourceManager) {}

    void work() override {
        //std::lock_guard<std::mutex> g(queueLock);
        queueLock.lock();
        while (!eventQueue.empty()) {
            //  NODECONTEXTLOGGER("Processing event");
            ///@todo better locking and unlocking of mutex

            EventTypePtr i = eventQueue.front();
            eventQueue.pop();
            queueLock.unlock();
            if (i != nullptr) {
                this->event(*i);
            } else {
                LOGGER("BAD POINTER");
            }


            queueLock.lock();

        }
        queueLock.unlock();

    }

    auto queueSize() {
        return eventQueue.size();
    }


};


#endif //BASYCO_EVENTQUEUESOURCE_H
