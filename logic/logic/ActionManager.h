//
// Created by stilgar on 25.08.17.
//

#ifndef BSC_ACTIONMANAGER_H
#define BSC_ACTIONMANAGER_H


#include <map>
#include "core/uber/Uber.h"
#include "core/signal/Signal.h"
#include "core/context/Context.h"


#include <optional>

namespace bsc {
    class ActionManager {
    private:
        Uber <std::map> actionMap;
    public:
        template<typename... Args>
        using Action = typename Signal<Args...>::Func;

        template<typename... Args>
        using ActionType = Action<Args...>;


        template<typename... Args, typename ActionIdType>
        std::optional<ActionType<Args...>> getAction(ActionIdType id) {
            auto& map = actionMap.get<ActionIdType, ActionType<Args...>>();
            if (map.count(id) > 0) {
                return map[id];
            } else {
                return std::nullopt;
            }

        }

        template<typename... Args, typename ActionIdType>
        void setAction(ActionIdType id, ActionType<Args...> action) {
            auto& map = actionMap.get<ActionIdType, ActionType<Args...>>();
            //@todo what if map[id] was already set?
            map[id] = action;
        }


    };
}


#endif //BSC_ACTIONMANAGER_H
