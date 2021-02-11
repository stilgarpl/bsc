//
// Created by Krzysztof Tulidowicz on 29.08.17.
//

#ifndef BSC_EVENTQUEUESOURCE_H
#define BSC_EVENTQUEUESOURCE_H

#include <log/log/Logger.h>
#include <logic/ISource.h>
#include <queue>

namespace bsc {
    template<typename EventType_, typename FriendClass = std::nullptr_t>
    class EventQueueSource : public ISource {
        friend FriendClass;

    protected:
        typedef EventType_ EventType;
        typedef typename EventType_::IdType EventIdType;

    public:
        typedef std::shared_ptr<EventType_> EventTypePtr;

    private:
        std::recursive_mutex queueLock;
        std::condition_variable_any queueReady;
        std::queue<EventTypePtr> eventQueue;

    protected:
        void queueEvent(EventTypePtr event) {
            {
                std::lock_guard g(queueLock);
                eventQueue.push(event);
            }
            queueReady.notify_one();
        }

        template<typename... Args>
        static EventTypePtr newEvent(Args... args) {
            return std::make_shared<EventType_>(args...);
        }
        bool isEventQueueEmpty() {
            std::lock_guard g(queueLock);
            return eventQueue.empty();
        }
        void stopRequested() override { queueReady.notify_all(); }

    public:
        explicit EventQueueSource(SourceManager& sourceManager) : ISource(sourceManager) {}

        void run() override {
            // std::lock_guard<std::mutex> g(queueLock);

            //@todo all that locking and unlocking has to go...

            while (!this->isStopping()) {
                std::unique_lock g(queueLock);
                //@todo 1s or other value?
                //                queueReady.wait_for(g, 1s, [&]() -> bool { return (!isEventQueueEmpty() ||
                //                this->isStopping()); });
                queueReady.wait_for(g, 100ms);
                while (!eventQueue.empty()) {
                    //  LOGGER("Processing event");
                    //@todo better locking and unlocking of mutex

                    EventTypePtr i = eventQueue.front();
                    eventQueue.pop();
                    // unlock so event execution won't block adding new things to queue
                    g.unlock();
                    if (i != nullptr) {
                        this->event(*i);
                    } else {
                        LOGGER("BAD POINTER");
                    }
                    g.lock();
                }
            }

        }

        auto queueSize() {
            return eventQueue.size();
        }

        ~EventQueueSource() = default;
    };
}


#endif //BSC_EVENTQUEUESOURCE_H
