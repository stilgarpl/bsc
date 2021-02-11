//
// Created by Krzysztof Tulidowicz on 14.10.2019.
//

#ifndef BSC_STATEOBSERVER_H
#define BSC_STATEOBSERVER_H

#include <log/log/Logger.h>
#include <memory>
#include <mutex>

namespace bsc {

    template<typename StateObject, typename stateIdType>
    class Observer;

    template<typename StateObject, typename stateIdType>
    class Observee {
    public:
        virtual void unregisterStateObserver(Observer<StateObject, stateIdType>& observer) = 0;
    };

    template<typename StateObject, typename stateIdType>
    class Observer : public std::enable_shared_from_this<Observer<StateObject, stateIdType>> {
    public:
        using ObserveeType = Observee<StateObject, stateIdType>;
    private:
        std::recursive_mutex observeeMutex;
        //this should be setDirect to null in observee destructor
        //@todo I don't like raw pointer here...
        ObserveeType* observee = nullptr;
    public:
        //@todo setObservee method should be private. find a way to friend this to NotifyMethod
        void setObservee(ObserveeType* o) {
            std::lock_guard<std::recursive_mutex> g(observeeMutex);
            observee = o;
        }

    public:
        virtual void update(StateObject& object, stateIdType) = 0;

        virtual ~Observer() {
            std::lock_guard<std::recursive_mutex> g(observeeMutex);
            if (observee) {
                LOGGER("unregistering observer")
                observee->unregisterStateObserver(*this);
            }
        };

        friend ObserveeType;
    };
}

#endif //BSC_STATEOBSERVER_H