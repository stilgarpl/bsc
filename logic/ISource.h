//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_SOURCE_H
#define BASYCO_SOURCE_H


#include "../signal/Signal.h"
#include "../context/Context.h"
#include "../uber/Uber.h"


class ISource {
private:
    Context context;
    Uber<std::map> signalMap;
    Uber<type> globalSignal;
protected:
    template<typename T>
    void event(const T &event) {
        this->getSignal<T>(event.getEventId()).signal(context, event);
        this->getSignal<T>().signal(context, event);
    }


public:

    template<typename T>
    using SignalType = Signal<Context &, const T &>;

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
//        typedef Signal<Context&, const T&> SignalType;
//        typedef std::shared_ptr<SignalType> SignalTypePtr;

        return globalSignal.get<SignalType<T>>();


    }

    Context &getContext();

    virtual void work() =0;

    ///@todo maybe later
//    template<typename T>
//    Signal<Context&, const T&>& assignSignal(const typename T::IdType& id,typename Signal<Context&, const T&>::FuncPtr funcPtr ) {
//        this->getSignal<T>(id).assign(funcPtr);
//    };
//
//    template<typename T>
//    Signal<Context&, const T&>& assignSignal(const typename T::IdType& id,const typename Signal<Context&, const T&>::Func& funcPtr ) {
//        this->getSignal<T>(id).assign(funcPtr);
//    };
};


#endif //BASYCO_SOURCE_H
