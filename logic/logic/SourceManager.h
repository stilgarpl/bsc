//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_SOURCEMANAGER_H
#define BASYCO_SOURCEMANAGER_H



#include <list>
#include <memory>
#include <thread>
#include <core/signal/Signal.h>
#include <core/context/Context.h>
#include <core/uber/Uber.h>
#include <core/signal/OrderedExecutionPolicy.h>

using namespace std::chrono_literals;

class ISource;

class SourceManager {
public:
    SourceManager();

public:
    typedef std::shared_ptr<ISource> SourcePtr;
    typedef std::list<SourcePtr> SourceList;

    template<typename T, typename... Args>
    using SignalType = Signal<const T &, Args...>;

    //@todo dlaczego właściwie te sygnały w mapie są jako pointery?
    /// jeśli mogą być niezdefiniowane, no to trzeba robic std::optional
    ///ale ja zwracam referki i tak, więc moze lepiej w mapie przechowywac te sygnaly tak po prostu?
    template<typename T, typename... Args>
    using SignalTypePtr = std::shared_ptr<SignalType<T, Args...>>;

    template<typename T, typename... Args>
    using SignalMapType = std::map<typename T::IdType, SignalTypePtr<T, Args...>>;

protected:

private:
    Uber<std::map> signalMap;
    Uber<Type> globalSignal;
    StaticUber<std::shared_ptr<ExecutionPolicy>> executionPolicy;
    std::mutex signalMapMutex;

protected:

    template<typename EventType>
    std::shared_ptr<ExecutionPolicy> getExecutionPolicy() {
        return executionPolicy.get<EventType>();
    }

public:
    template<typename EventType>
    void setExecutionPolicy(std::shared_ptr<ExecutionPolicy> policy) {
        if (executionPolicy.get<EventType>() == nullptr) {
            executionPolicy.get<EventType>() = policy;
        }
    }

public:

    void startSources();

    void stopSources();

    void joinSources();

    template<typename EventType, typename... Args>
    void event(const EventType &event, Args... args) {

        //@todo pass to executor
//        Context::setActiveContext(&commonContext);
//        Context::Ptr previousContext = Context::getActiveContext();
//        Context::setActiveContext(event.context());
        SetLocalContext localContext(event.context());

        auto policy = getExecutionPolicy<EventType>();
        if (policy != nullptr) {
            std::shared_ptr<Executor> executor = policy->executor();
            /*int b =*/ this->getSignal<EventType, Args...>().signal(executor, event, args...);
            /*int a =*/ this->getSignal<EventType, Args...>(event.getEventId()).signal(executor, event, args...);
        } else {
            //@todo or just add default executor here somewhere...
            /*int b =*/ this->getSignal<EventType, Args...>().signal(event, args...);
            /*int a =*/ this->getSignal<EventType, Args...>(event.getEventId()).signal(event, args...);
        }

    }


    template<typename EventType, typename... Args>
    SignalMapType<EventType, Args...> &getSignalMap() {
        return signalMap.get<typename EventType::IdType, SignalTypePtr<EventType, Args...>>();
    }


    template<typename EventType, typename... Args>
    SignalType<EventType, Args...> &getSignal(const typename EventType::IdType &id) {
        std::lock_guard<std::mutex> g(signalMapMutex);
        // typedef Signal<Context&, const GroupType&> SignalType;
        //   typedef std::shared_ptr<SignalType> SignalTypePtr;
        auto &map = getSignalMap<EventType, Args...>();//signalMap.get<typename GroupType::IdType,SignalTypePtr<GroupType>>();
        if (map[id] == nullptr) {
            map[id] = std::make_shared<SignalType<EventType, Args...>>();
        }

        return *(map[id]);

    }

    template<typename EventType, typename... Args>
    SignalType<EventType, Args...> &getSignal() {
        return globalSignal.get<SignalType<EventType, Args...>>();
    }


    template<typename EventType, typename... Args>
    SignalType<EventType, Args...> &
    assignSignal(const typename EventType::IdType &id, typename SignalType<EventType, Args...>::FuncPtr funcPtr) {
        return this->getSignal<EventType, Args...>(id).assign(funcPtr);
    }

    template<typename EventType, typename... Args>
    SignalType<EventType, Args...> &
    assignSignal(const typename EventType::IdType &id, const typename SignalType<EventType, Args...>::Func &funcPtr) {
        return this->getSignal<EventType, Args...>(id).assign(funcPtr);
    }


    template<typename EventType, typename... Args>
    SignalType<EventType, Args...> &
    assignSignal(typename SignalType<EventType, Args...>::FuncPtr funcPtr) {
        return this->getSignal<EventType, Args...>().assign(funcPtr);
    }

    template<typename EventType, typename... Args>
    SignalType<EventType, Args...> &
    assignSignal(const typename SignalType<EventType, Args...>::Func &funcPtr) {
        return this->getSignal<EventType, Args...>().assign(funcPtr);
    }







//
private:
    SourceList sources;
    //@todo mozna zmienic ten type na list jesli wiecej niz jedno source danego typu bedzie potrzebne
    Uber<Type> sourcesByType;
    //@todo it would appear that the common context is not actually used - it is replaced by context from events.
    Context::OwnPtr commonContext = Context::makeContext();
public:


    template<typename SourceType, typename... Args>
    void addSource(Args... args) {
        std::shared_ptr<SourceType> sourcePtr = std::make_shared<SourceType>(*this, args...);
        addSource(sourcePtr);
        auto &sbt = sourcesByType.get<std::shared_ptr<SourceType>>();
        sbt = sourcePtr;
    }

    template<typename SourceType>
    std::shared_ptr<SourceType> getSource() {
        return sourcesByType.get<std::shared_ptr<SourceType>>();
    }

    template<typename SourceType>
    void removeSource() {
        std::shared_ptr<SourceType> sourcePtr = sourcesByType.get<std::shared_ptr<SourceType>>();
        removeSource(sourcePtr);
        sourcesByType.get<std::shared_ptr<SourceType>>() = nullptr;
    }

protected:
    void addSource(const SourcePtr &source);

    void removeSource(SourcePtr source) {
        sources.remove(source);
    }

public:
    template<typename EventType, typename... Args>
    int registerTrigger(const typename SignalType<EventType, Args...>::Func &func) {
        int assigned = 1;
//        for (auto &&it : getProviders<EventType, Args...>()) {
        getSignal<EventType, Args...>().assign(func);
//            assigned++;
//        }
        return assigned;
    }

    template<typename EventType, typename... Args>
    int
    registerTrigger(const typename EventType::IdType &id,
                    const typename SignalType<EventType, Args...>::Func &func) {
        int assigned = 1;
//        for (auto &&it : getProviders<EventType, Args...>()) {
        getSignal<EventType, Args...>(id).assign(func);
//            assigned++;

//        }
        return assigned;
    }

//    void setContexts(const Context &context) {
//        commonContext += context;
//        for (auto &&it : sources) {
//            it->setContext(context);
//        }
//    }
    void setContext(const Context::Ptr &context);


};


#endif //BASYCO_SOURCEMANAGER_H