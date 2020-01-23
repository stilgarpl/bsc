//
// Created by stilgar on 14.10.2019.
//

#ifndef BSC_DIRECTNOTIFY_H
#define BSC_DIRECTNOTIFY_H


namespace bsc {
    template<typename StateObject, typename stateIdType>
    class DirectNotify {
        using ObserverType = Observer<StateObject, stateIdType>;
        std::list<std::reference_wrapper<ObserverType>> observers;
        std::recursive_mutex observersLock;
    public:
        void registerObserver(ObserverType& observer) {
            std::unique_lock g(observersLock);
            observers.push_back(observer);
        }

        void unregisterObserver(ObserverType& observer) {
            std::unique_lock g(observersLock);
            auto before = observers.size();
            observers.erase(std::remove_if(observers.begin(), observers.end(),
                                           [&](auto& i) -> bool {
                                               return (&i.get() == &observer);

                                           }), observers.end());
            auto after = observers.size();
            LOGGER("removed " + std::to_string(before - after))
        }

        void notify(StateObject& object, stateIdType state) {
            std::unique_lock guard(observersLock);
            LOGGER(std::string("notify: ") + typeid(StateObject).name() + " : " + std::to_string(observers.size()))
            auto observersCopy = observers;
            guard.unlock(); //@todo not sure if I can run update() without this locked. we'll see.
            for (const auto& observer : observersCopy) {
                auto& o = observer.get();
                o.update(object, state);
            }
            LOGGER("notified")
        }

        virtual ~DirectNotify() {
            auto observersCopy = observers;
            for (auto& observer : observersCopy) {
                observer.get().setObservee(nullptr);
            }
        }
    };
}

#endif //BSC_DIRECTNOTIFY_H
