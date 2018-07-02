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

    template<typename EventType, typename ActionId>
    bool assignAction(ActionId actionId) {
        auto &&action = actionManager.getAction<EventType>(actionId);
//        if (!sourceManager.hasProvider<EventType>()) {
//            std::cerr << "Provider doesn't exist: " << typeid(EventType).name() << std::endl;
//        }
        if (action) {
            sourceManager.registerTrigger<EventType>(*action);
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
    template<typename SourceType>
    std::shared_ptr<SourceType> getSource() {
        return sourceManager.getSource<SourceType>();
    }

    template<typename SourceType>
    void removeSource() {
        sourceManager.removeSource<SourceType>();
    }


    template<typename SourceType, typename... Args>
    void requireSource(Args... args) {
        if (getSource<SourceType>() == nullptr) {
            sourceManager.addSource<SourceType>(args...);
        }
    }

    void work() {
        sourceManager.work();
    }


    template<typename... Args, typename ActionIdType>
    void setAction(ActionIdType id, ActionManager::ActionType<Args...> action) {
        actionManager.setAction<Args...>(id, action);
    };

    LogicManager(const LogicManager &) = delete;

    LogicManager() = default;

    void run() override {
        while (!isStopping()) {
            work();
            //  std::this_thread::sleep_for(0s);
        }

    }

    void setContexts(const Context &context) {
        sourceManager.setContext(context);
    }

    virtual ~LogicManager() {
        stop();
        join();
    }
};


#endif //BASYCO_LOGICMANAGER_H
