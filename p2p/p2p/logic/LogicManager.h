#include <utility>

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
        auto &&action = actionManager.getAction<const typename std::decay<EventType>::type &, Args...>(actionId);
        if (action) {
            auto ret = sourceManager.registerTrigger<EventType, Args...>(eventId, *action);
            return ret > 0;

        } else {
            return false;
        }
    }

    template<typename EventType, typename... Args, typename ActionId>
    auto getAction(ActionId actionId) {
        return actionManager.getAction<const typename std::decay<EventType>::type &, Args...>(actionId);
    }

    template<typename EventType, typename... Args, typename ActionId>
    bool assignAction(ActionId actionId) {
        auto &&action = actionManager.getAction<const typename std::decay<EventType>::type &, Args...>(actionId);
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



    template<typename EventType, typename... Args, typename ActionIdType>
    void setAction(ActionIdType id,
                   ActionManager::ActionType<const typename std::decay<EventType>::type &, Args...> action) {
        actionManager.setAction<const typename std::decay<EventType>::type &, Args...>(id, action);
    }

    //@todo this could probably removed and I could just emit events from chains directly using autosource... but maybe it's useful?
    template<typename RetType, typename EventType, typename... Args, typename ActionIdType>
    void setActionExtended(ActionIdType id, std::function<typename std::decay<RetType>::type(
            const typename std::decay<EventType>::type &, Args...)> func) {
        actionManager.setAction<const typename std::decay<EventType>::type &, Args...>(id, [=, this](
                const typename std::decay<EventType>::type &e, Args... args) {
            //@todo by value?
            RetType ret = func(e, args...);
            if (ret.isEventValid()) {
                sourceManager.event(ret);
            }
        });
    }

    LogicManager(const LogicManager &) = delete;

    LogicManager() = default;

    void run() override;

    void onStop() override;

    void onStart() override;

    void setContexts(const Context::Ptr &context);

    ~LogicManager() override;


};


#endif //BASYCO_LOGICMANAGER_H
