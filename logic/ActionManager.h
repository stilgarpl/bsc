//
// Created by stilgar on 25.08.17.
//

#ifndef BASYCO_ACTIONMANAGER_H
#define BASYCO_ACTIONMANAGER_H


#include <map>
#include "uber/Uber.h"
#include "signal/Signal.h"
#include "context/Context.h"
#include <experimental/optional>

class ActionManager {
private:
    Uber<std::map> actionMap;
public:
    template<typename... Args>
    using Action = typename Signal<Args...>::Func;

    template<typename... Args>
    using ActionType = Action<Args...>;


    template<typename... Args, typename ActionIdType>
    std::experimental::optional<ActionType<Args...>> getAction(ActionIdType id) {
        auto &map = actionMap.get<ActionIdType, ActionType<Args...>>();
        if (map.count(id) > 0) {
            return map[id];
        } else {
            return std::experimental::nullopt;
        }

    }

    template<typename... Args, typename ActionIdType>
    void setAction(ActionIdType id, ActionType<Args...> action) {
        auto &map = actionMap.get<ActionIdType, ActionType<Args...>>();
        map[id] = action;
    };


};


#endif //BASYCO_ACTIONMANAGER_H
