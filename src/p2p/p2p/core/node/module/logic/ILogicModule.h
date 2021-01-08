//
// Created by Krzysztof Tulidowicz on 12.10.17.
//

#ifndef BSC_ILOGICMODULE_H
#define BSC_ILOGICMODULE_H


#include <logic/sources/AutoSource.h>
#include <p2p/core/node/module/INodeModule.h>
#include <p2p/core/node/INode.h>
#include <p2p/core/node/module/ModuleState.h>
#include <logic/state/LogicStateMachine.h>
#include <logic/LogicManager.h>
#include <logic/LogicObject.h>
#include <logic/state/DirectNotify.h>


namespace bsc {
    class ILogicModule : public INodeModule, public LogicObject, public LogicStateMachine<ILogicModule, ModuleState, DirectNotify> {

    public:
        bool setupLogic() override { return LogicObject::setupLogic(); }

    private:
        static DefinitionPtr makeDefinition() {
            static auto ptr = std::make_shared<Definition>([]() {
                Definition definition;
                definition.addState(ModuleState::UNINITIALIZED,
                                    ModuleState::INITIALIZED,
                                    ModuleState::LOGIC_READY,
                                    ModuleState::SUBMODULES_PREPARED,
                                    ModuleState::READY,
                                    ModuleState::SHUTDOWN);
                definition.addLink(ModuleState::UNINITIALIZED, ModuleState::INITIALIZED);
                definition.addLink(ModuleState::INITIALIZED, ModuleState::LOGIC_READY, ModuleState::SHUTDOWN);
                definition.addLink(ModuleState::LOGIC_READY, ModuleState::SUBMODULES_PREPARED, ModuleState::SHUTDOWN);
                definition.addLink(ModuleState::SUBMODULES_PREPARED, ModuleState::READY);
                definition.addLink(ModuleState::READY, ModuleState::SHUTDOWN);
                return definition;
            }());
            return ptr;
        }

    public:
        explicit ILogicModule(INode& node)
            : INodeModule(node), LogicObject(node.getLogicManager()), LogicStateMachine(makeDefinition(), *this) {

            setState(ModuleState::UNINITIALIZED);
        }

        template<typename EventType, typename... Args>
        class ModuleLogicChainHelper : public LogicObject::SpecificLogicChainHelper<ModuleLogicChainHelper, EventType, Args...> {
            //        typedef ModuleLogicChainHelper<EventType, Args...> ThisType;
        private:
            INode& node;
        public:
            //        @todo figure out how to get module pointer from this place
            template<typename RetType, typename ModuleType, typename... ModArgs>
            ThisType& fireModuleAction(RetType (ModuleType::*f)(ModArgs... args)) {
                std::cout << "assigning module action ... " << std::endl;
                //@todo c++ bind_front? or sth? unpack placeholders to the sizeof ModArgs?
                if constexpr  (sizeof...(ModArgs) == 0) {
                    fireNewAction(std::bind(f, node.getModule<ModuleType>()));
                } else if constexpr (sizeof...(ModArgs) == 1) {
                    fireNewAction(
                            std::bind(f, node.getModule<ModuleType>(), std::placeholders::_1));
                } else if constexpr (sizeof...(ModArgs) == 2) {
                    fireNewAction(
                            std::bind(f, node.getModule<ModuleType>(), std::placeholders::_1, std::placeholders::_2));
                } else if constexpr (sizeof...(ModArgs) == 3) {
                    fireNewAction(
                            std::bind(f, node.getModule<ModuleType>(), std::placeholders::_1, std::placeholders::_2,
                                      std::placeholders::_3));
                }
                return *this;
            }

            ModuleLogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l,
                                   LogicObject& lo,
                                   INode& node)
                    : SpecificLogicChainHelper<ModuleLogicChainHelper, EventType, Args...>(eventHelper, l, lo),
                      node(node) {}

            template<typename NewThisType>
            ModuleLogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l,
                                   LogicObject& lo,
                                   const NewThisType& self)
                    : SpecificLogicChainHelper<ModuleLogicChainHelper, EventType, Args...>(eventHelper, l, lo, self),
                      node(self.node) {

            }

            template<typename NewEventType, typename ... NewArgs>
            friend
            class ModuleLogicChainHelper;

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
    protected:

        template<typename EventType, typename ... Args>
        ModuleLogicChainHelper<EventType, Args...> when(const EventHelper<EventType, Args...> eventHelper) {
            return ModuleLogicChainHelper<EventType, Args...>(eventHelper, logicManager, *this, node);
        }

        //    template<typename EventType, typename ... Args>
        //    StateModuleLogicChainHelper<EventType, Args...> when(const StateHelper<EventType, Args...> eventHelper) {
        //        return StateModuleLogicChainHelper<EventType, Args...>(eventHelper, logicManager, node);
        //    }



    protected:

        //when(event<CommandEvent>().withId(CommandId::EXECUTE_COMMAND)).fireNewAction(RUN_COMMAND);d

        void changeModuleState(const ModuleState& state) override {
            changeState(state);
        }


    };
}


#endif //BSC_ILOGICMODULE_H
