//
// Created by stilgar on 25.08.17.
//

#ifndef BASYCO_LOGICMANAGER_H
#define BASYCO_LOGICMANAGER_H


#include "SourceManager.h"
#include "ActionManager.h"

class LogicManager {

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
        auto &&action = actionManager.getAction<EventType, Args...>(actionId);
        if (action) {
            sourceManager.registerTrigger<EventType, Args...>(eventId, *action);
            return true;
        } else {
            return false;
        }
    }

    template<typename EventType, typename ActionId>
    bool assignAction(ActionId actionId) {
        auto &&action = actionManager.getAction<EventType>(actionId);
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


    template<typename SourceType, typename... Args>
    void addSource(Args... args) {
        sourceManager.addSource<SourceType>(args...);
    }

    template<typename SourceType>
    SourceType &getSource() {
        return sourceManager.getSource<SourceType>();
    }

    template<typename SourceType>
    void removeSource() {
        sourceManager.removeSource<SourceType>();
    }


    void work() {
        sourceManager.work();
    }


    template<typename... Args, typename ActionIdType>
    void setAction(ActionIdType id, ActionManager::ActionType<Args...> action) {
        actionManager.setAction<Args...>(id, action);
    };
};


#endif //BASYCO_LOGICMANAGER_H
