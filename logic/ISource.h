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
    ///@todo rozwazyc variadic templatexc
    template<typename T>
    void event(const T &event) {
        this->getSignal<T>(event.getEventId()).signal(context, event);
        this->getSignal<T>().signal(context, event);
    }


public:

    template<typename T>
    using SignalType = Signal<Context &, const T &>;

    ///@todo dlaczego właściwie te sygnały w mapie są jako pointery?
    /// jeśli mogą być niezdefiniowane, no to trzeba robic std::optional
    ///ale ja zwracam referki i tak, więc moze lepiej w mapie przechowywac te sygnaly tak po prostu?
    template<typename T>
    using SignalTypePtr = std::shared_ptr<SignalType<T>>;

    template<typename T>
    using SignalMapType = std::map<typename T::IdType, SignalTypePtr<T>>;


    template<typename T>
    SignalMapType<T> &getSignalMap() {
        return signalMap.get<typename T::IdType, SignalTypePtr<T>>();
    }


    template<typename T>
    SignalType<T> &getSignal(const typename T::IdType &id) {
        // typedef Signal<Context&, const T&> SignalType;
        //   typedef std::shared_ptr<SignalType> SignalTypePtr;
        auto &map = getSignalMap<T>();//signalMap.get<typename T::IdType,SignalTypePtr<T>>();
        if (map[id] == nullptr) {
            map[id] = std::make_shared<SignalType<T>>();
        }

        return *(map[id]);

    }

    template<typename T>
    Signal<Context &, const T &> &getSignal() {
        return globalSignal.get<SignalType<T>>();
    }

    Context &getContext();

    virtual void work() =0;

    template<typename T>
    SignalType<T> &assignSignal(const typename T::IdType &id, typename SignalType<T>::FuncPtr funcPtr) {
        this->getSignal<T>(id).assign(funcPtr);
    };

    template<typename T>
    SignalType<T> &assignSignal(const typename T::IdType &id, const typename SignalType<T>::Func &funcPtr) {
        this->getSignal<T>(id).assign(funcPtr);
    };


    virtual void registerProviders(SourceManager *);
};


#endif //BASYCO_SOURCE_H
