//
// Created by stilgar on 25.08.17.
//

#ifndef BASYCO_ACTIONMANAGER_H
#define BASYCO_ACTIONMANAGER_H


#include <map>
#include "core/uber/Uber.h"
#include "core/signal/Signal.h"
#include "core/context/Context.h"


#include <optional>

class ActionManager {
private:
    bsc::Uber<std::map> actionMap;
public:
    template<typename... Args>
    using Action = typename bsc::Signal<Args...>::Func;

    template<typename... Args>
    using ActionType = Action<Args...>;


    template<typename... Args, typename ActionIdType>
    std::optional<ActionType<Args...>> getAction(ActionIdType id) {
        auto &map = actionMap.get<ActionIdType, ActionType<Args...>>();
        if (map.count(id) > 0) {
            return map[id];
        } else {
            return std::nullopt;
        }

    }

    template<typename... Args, typename ActionIdType>
    void setAction(ActionIdType id, ActionType<Args...> action) {
        auto &map = actionMap.get<ActionIdType, ActionType<Args...>>();
        //@todo what if map[id] was already setDirect?
        map[id] = action;
    }


};


#endif //BASYCO_ACTIONMANAGER_H
