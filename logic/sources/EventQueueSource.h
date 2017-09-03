//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_EVENTQUEUESOURCE_H
#define BASYCO_EVENTQUEUESOURCE_H


#include <queue>
#include "../ISource.h"
#include "../SourceManager.h"

template<typename EventType>
class EventQueueSource : public ISource {
public:
    typedef std::shared_ptr<EventType> EventTypePtr;
private:
    ISource *owner = nullptr;
    ///@todo thread safety
    std::queue<EventTypePtr> eventQueue;
public: //@todo na pewno public?
    void queueEvent(EventTypePtr event) {
        eventQueue.push(event);
    }

public:
    void work() override {

        while (!eventQueue.empty()) {
            std::clog << __func__ << " processing event" << std::endl;

            auto &i = eventQueue.front();
            this->event(*i);
//            if(owner!= nullptr) {
//                owner->event(*i);
//            }

            eventQueue.pop();
        }

    }

    void registerProviders(SourceManager *manager) override {
        manager->registerProvider<EventType>();
    }

    ISource *getOwner() const {
        return owner;
    }

    void setOwner(ISource *owner) {
        EventQueueSource::owner = owner;
    }

    EventQueueSource(ISource *owner) : owner(owner) {}

    EventQueueSource() = default;
};


#endif //BASYCO_EVENTQUEUESOURCE_H
