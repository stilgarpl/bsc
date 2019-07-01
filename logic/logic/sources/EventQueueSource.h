//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_EVENTQUEUESOURCE_H
#define BASYCO_EVENTQUEUESOURCE_H


#include <queue>
#include "../ISource.h"
#include "../IEvent.h"
#include "core/log/Logger.h"
//#include "p2p/modules/nodeNetworkModule/protocol/context/NodeContext.h"

template<typename EventType_, typename FriendClass=std::nullptr_t>
class EventQueueSource : public ISource {
    friend FriendClass;
protected:
    typedef EventType_ EventType;
    typedef typename EventType_::IdType EventIdType;
public:
    typedef std::shared_ptr<EventType_> EventTypePtr;
private:
    std::mutex queueLock;
    std::condition_variable queueReady;
    std::queue<EventTypePtr> eventQueue;
protected:
    void queueEvent(EventTypePtr event) {
        std::lock_guard<std::mutex> g(queueLock);
        eventQueue.push(event);
        queueReady.notify_one();
    }

    template<typename ... Args>
    EventTypePtr newEvent(Args... args) {
        return std::make_shared<EventType_>(args...);
    }
public:
    explicit EventQueueSource(SourceManager &sourceManager) : ISource(sourceManager) {}


    void run() override {
        //std::lock_guard<std::mutex> g(queueLock);
        std::unique_lock<std::mutex> g(queueLock);
        g.unlock();
        while (!this->isStopping()) {
            g.lock();
            //@todo 1s or other value?
            queueReady.wait_for(g, 1s, [&]() -> bool { return (!eventQueue.empty() || this->isStopping()); });
            while (!eventQueue.empty()) {
                //  NODECONTEXTLOGGER("Processing event");
                //@todo better locking and unlocking of mutex

                EventTypePtr i = eventQueue.front();
                eventQueue.pop();
                //unlock so event execution won't block adding new things to queue
                g.unlock();
                if (i != nullptr) {
                    this->event(*i);
                } else {
                    LOGGER("BAD POINTER");
                }
                g.lock();
            }
            g.unlock();
        }

    }

    auto queueSize() {
        return eventQueue.size();
    }

    ~EventQueueSource() override {
        stop();
        queueReady.notify_all();
    }
};


#endif //BASYCO_EVENTQUEUESOURCE_H
