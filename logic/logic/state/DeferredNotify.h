//
// Created by stilgar on 14.10.2019.
//

#ifndef BASYCO_DEFERREDNOTIFY_H
#define BASYCO_DEFERREDNOTIFY_H

#include "LogicStateMachine.h"


template<typename StateObject, typename stateIdType>
class DeferredNotify {
    using ObserverType = Observer<StateObject, stateIdType>;
    std::list<std::shared_ptr<ObserverType>> observers;
    std::unique_ptr<ThreadQueueProcessor<stateIdType, StateObject>> processor;
    std::mutex observerLock;
public:

    DeferredNotify() {
        processor = std::make_unique<ThreadQueueProcessor<stateIdType, StateObject>>([this](auto state, auto& object) {
            auto observersCopy = observers;
            for (const auto& observer : observersCopy) {
                observer->update(object, state);
            }
        });
    }

    void registerObserver(ObserverType& observer) {
        std::unique_lock g(observerLock);
        auto sharedObserver = observer.shared_from_this();
        //@todo change this assert to something better or remove
        assert(sharedObserver != nullptr);
        observers.push_back(sharedObserver);
    }

    void unregisterObserver(ObserverType& observer) {
        std::unique_lock g(observerLock);
        auto before = observers.size();
        observers.erase(std::remove_if(observers.begin(), observers.end(),
                                       [&](auto& i) -> bool {
                                           return (i.get() == &observer);

                                       }), observers.end());
        auto after = observers.size();
        LOGGER("removed " + std::to_string(before - after))
    }

    void notify(StateObject& object, stateIdType state) {
//        std::unique_lock guard(notifyLock);
//@todo atomic_shared_ptr?
        processor->process(state, object);
    }

    virtual ~DeferredNotify() {
//        std::unique_lock guard(notifyLock);
        //will stop the processor and join its thread
        processor = nullptr;

    }
};

#endif //BASYCO_DEFERREDNOTIFY_H
