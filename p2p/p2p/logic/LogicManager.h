//
// Created by stilgar on 25.08.17.
//

#ifndef BASYCO_LOGICMANAGER_H
#define BASYCO_LOGICMANAGER_H


#include "SourceManager.h"
#include "ActionManager.h"
#include "p2p/thread/Runnable.h"

class LogicManager : public Runnable {

private:
    SourceManager sourceManager;
    ActionManager actionManager;
protected:
    SourceManager &getSourceManager() {
        return sourceManager;
    }

    ActionManager &getActionManager() {
        return actionManager;
    }

public:

    template<typename EventType, typename... Args, typename ActionId>
    bool assignAction(typename EventType::IdType eventId, ActionId actionId) {
//        if (!sourceManager.hasProvider<EventType, Args...>()) {
//            std::cerr << "Provider doesn't exist: " << typeid(EventType).name() << std::endl;
//            return false;
//        }
        auto &&action = actionManager.getAction<EventType, Args...>(actionId);
        if (action) {
            auto ret = sourceManager.registerTrigger<EventType, Args...>(eventId, *action);
            return ret > 0;

        } else {
            return false;
        }
    }

    template<typename EventType, typename... Args, typename ActionId>
    auto getAction(ActionId actionId) {
        return actionManager.getAction<EventType, Args...>(actionId);
    }

    template<typename EventType, typename... Args, typename ActionId>
    bool assignAction(ActionId actionId) {
        auto &&action = actionManager.getAction<EventType, Args...>(actionId);
//        if (!sourceManager.hasProvider<EventType>()) {
//            std::cerr << "Provider doesn't exist: " << typeid(EventType).name() << std::endl;
//        }
        if (action) {
            sourceManager.registerTrigger<EventType, Args...>(*action);
            return true;
        } else {
            return false;
        }
    }

//    void addSource(std::shared_ptr<ISource> source) {
//        sourceManager.addSource(source);
//    }

protected:
    template<typename SourceType, typename... Args>
    void addSource(Args... args) {
        sourceManager.addSource<SourceType>(args...);
    }

public:

    template<typename EventType>
    void setExecutionPolicy(std::shared_ptr<ExecutionPolicy> policy) {
        sourceManager.setExecutionPolicy<EventType>(policy);
    }

    template<typename SourceType>
    std::shared_ptr<SourceType> getSource() {
        return sourceManager.getSource<SourceType>();
    }

    template<typename SourceType>
    void removeSource() {
        sourceManager.removeSource<SourceType>();
    }


    template<typename SourceType, typename... Args>
    SourceType &requireSource(Args... args) {
        if (getSource<SourceType>() == nullptr) {
            sourceManager.addSource<SourceType>(args...);
        }
        return *getSource<SourceType>();
    }

    void work() {
        sourceManager.work();
    }


    template<typename... Args, typename ActionIdType>
    void setAction(ActionIdType id, ActionManager::ActionType<Args...> action) {
        actionManager.setAction<Args...>(id, action);
    }

    template<typename RetType, typename... Args, typename ActionIdType>
    void setActionExtended(ActionIdType id, std::function<typename std::decay<RetType>::type(Args...)> func) {
        actionManager.setAction<Args...>(id, [=](Args... args) {
            //@todo by value?
            RetType ret = func(args...);
            if (ret.isEventValid()) {
                sourceManager.event(ret);
            }
        });
    }

    LogicManager(const LogicManager &) = delete;

    LogicManager() = default;

    void run() override {
        while (!isStopping()) {
            //@todo cos wymyslec, zebyt to nie zżerało 100% cpu
            work();
            //  std::this_thread::sleep_for(0s);
            std::this_thread::sleep_for(1ms);
        }

    }

    void setContexts(Context::Ptr context) {
        sourceManager.setContext(context);
    }

    virtual ~LogicManager() {
        stop();
        join();
    }

//
//    //@todo think about it -> I'm not convinced event generation should be here (events should be fired from sources), but it's required by ILogicModule smart logic setup (I could make a separate source just for that... )
//    template<typename T, typename... Args>
//    void event(const T &event, Args... args) {
//
//        sourceManager.event<T, Args...>(event, args...);
//    }

};


#endif //BASYCO_LOGICMANAGER_H
