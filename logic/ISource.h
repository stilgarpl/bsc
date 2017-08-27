//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_SOURCE_H
#define BASYCO_SOURCE_H


#include "../signal/Signal.h"
#include "../context/Context.h"
#include "../uber/Uber.h"


///@todo Clion jest głupi i uważa, że to jest pełna definicja.
class SourceManager;

class ISource {
private:
    Context context;
    Uber<std::map> signalMap;
    Uber<Type> globalSignal;
protected:


    template<typename T, typename... Args>
    void event(const T &event, Args... args) {
        this->getSignal<T, Args...>(event.getEventId()).signal(context, event, args...);
        this->getSignal<T, Args...>().signal(context, event, args...);
    }


public:

    template<typename T, typename... Args>
    using SignalType = Signal<Context &, const T &, Args...>;

    ///@todo dlaczego właściwie te sygnały w mapie są jako pointery?
    /// jeśli mogą być niezdefiniowane, no to trzeba robic std::optional
    ///ale ja zwracam referki i tak, więc moze lepiej w mapie przechowywac te sygnaly tak po prostu?
    template<typename T, typename... Args>
    using SignalTypePtr = std::shared_ptr<SignalType<T, Args...>>;

    template<typename T, typename... Args>
    using SignalMapType = std::map<typename T::IdType, SignalTypePtr<T, Args...>>;


    template<typename T, typename... Args>
    SignalMapType<T, Args...> &getSignalMap() {
        return signalMap.get<typename T::IdType, SignalTypePtr<T, Args...>>();
    }


    template<typename T, typename... Args>
    SignalType<T, Args...> &getSignal(const typename T::IdType &id) {
        // typedef Signal<Context&, const T&> SignalType;
        //   typedef std::shared_ptr<SignalType> SignalTypePtr;
        auto &map = getSignalMap<T, Args...>();//signalMap.get<typename T::IdType,SignalTypePtr<T>>();
        if (map[id] == nullptr) {
            map[id] = std::make_shared<SignalType<T, Args...>>();
        }

        return *(map[id]);

    }

    template<typename T, typename... Args>
    SignalType<T, Args...> &getSignal() {
        return globalSignal.get<SignalType<T, Args...>>();
    }

    Context &getContext();

    virtual void work() =0;

    template<typename T, typename... Args>
    SignalType<T, Args...> &
    assignSignal(const typename T::IdType &id, typename SignalType<T, Args...>::FuncPtr funcPtr) {
        this->getSignal<T, Args...>(id).assign(funcPtr);
    };

    template<typename T, typename... Args>
    SignalType<T, Args...> &
    assignSignal(const typename T::IdType &id, const typename SignalType<T, Args...>::Func &funcPtr) {
        this->getSignal<T, Args...>(id).assign(funcPtr);
    };


    //@todo make pure virtual
    virtual void registerProviders(SourceManager *);
};


#endif //BASYCO_SOURCE_H
