//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_ILOGICMODULE_H
#define BASYCO_ILOGICMODULE_H


#include <p2p/logic/sources/AutoSource.h>
#include <p2p/node/INodeModule.h>
#include <p2p/node/INode.h>
#include "p2p/logic/LogicManager.h"
#include "p2p/logic/LogicObject.h"

//@todo #warning move this class to node modules
class ILogicModule : public INodeModule, public LogicObject {

public:

    bool setupLogic() override {
        return LogicObject::setupLogic();
    }

    explicit ILogicModule(INode &node) : INodeModule(node), LogicObject(node.getLogicManager()) {

    }

    template<typename EventType, typename ... Args>
    class ModuleLogicChainHelper : public SpecificLogicChainHelper<EventType, Args...> {
        typedef ModuleLogicChainHelper<EventType, Args...> ThisType;
    private:
        INode &node;
    public:
//        @todo figure out how to get module pointer from this place
        template<typename RetType, typename ModuleType, typename... ModArgs>
        ThisType &fireModuleAction(RetType (ModuleType::*f)(ModArgs... args)) {
            std::cout << "assigning module action ... " << std::endl;
            //@todo c++ bind_front? or sth? unpack placeholders to the sizeof ModArgs?
            if constexpr  (sizeof...(ModArgs) == 0) {
                LogicChainHelper<EventType, Args...>::fireNewAction(std::bind(f, node.getModule<ModuleType>()));
            } else if constexpr (sizeof...(ModArgs) == 1) {
                LogicChainHelper<EventType, Args...>::fireNewAction(
                        std::bind(f, node.getModule<ModuleType>(), std::placeholders::_1));
            } else if constexpr (sizeof...(ModArgs) == 2) {
                LogicChainHelper<EventType, Args...>::fireNewAction(
                        std::bind(f, node.getModule<ModuleType>(), std::placeholders::_1, std::placeholders::_2));
            } else if constexpr (sizeof...(ModArgs) == 3) {
                LogicChainHelper<EventType, Args...>::fireNewAction(
                        std::bind(f, node.getModule<ModuleType>(), std::placeholders::_1, std::placeholders::_2,
                                  std::placeholders::_3));
            }
            return *this;
        }

        ModuleLogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l, INode &node)
                : SpecificLogicChainHelper<EventType, Args...>(eventHelper, l), node(node) {}
    };

//    template<typename EventType, typename ... Args>
//    class StateModuleLogicChainHelper : public ModuleLogicChainHelper<EventType, Args...> {
//    public:
//        typename LogicChainHelper<EventType, Args...>::ThisType& fireStateChangeReaction(std::function<void(typename EventType::Type&)> reaction) {
//            return LogicChainHelper<EventType, Args...>::fireNewAction([reaction](auto event){reaction(event.getObject());});
//        }
//
//        StateModuleLogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l, INode &node) : ModuleLogicChainHelper<EventType, Args...>(eventHelper,l,node){};
//    };

    template<typename EventType, typename ... Args>
    ModuleLogicChainHelper<EventType, Args...> when(const EventHelper<EventType, Args...> eventHelper) {
        return ModuleLogicChainHelper<EventType, Args...>(eventHelper, logicManager, node);
    }

//    template<typename EventType, typename ... Args>
//    StateModuleLogicChainHelper<EventType, Args...> when(const StateHelper<EventType, Args...> eventHelper) {
//        return StateModuleLogicChainHelper<EventType, Args...>(eventHelper, logicManager, node);
//    }



protected:

    //when(event<CommandEvent>().withId(CommandId::EXECUTE_COMMAND)).fireNewAction(RUN_COMMAND);d




};


#endif //BASYCO_ILOGICMODULE_H
