find_package(spdlog CONFIG REQUIRED)

set(SOURCE_FILES logic/ISource.cpp logic/ISource.h logic/IEvent.h logic/SourceManager.cpp logic/SourceManager.h logic/sources/ClockSource.cpp logic/sources/ClockSource.h logic/events/Tick.cpp logic/events/Tick.h logic/ActionManager.cpp logic/ActionManager.h logic/LogicManager.cpp logic/LogicManager.h logic/sources/EventQueueSource.h logic/sources/AutoSource.cpp logic/sources/AutoSource.h logic/LogicObject.cpp logic/LogicObject.h logic/state/StateMachine.cpp logic/state/StateMachine.h logic/state/LogicStateMachine.cpp logic/state/LogicStateMachine.h logic/sources/LogicStateSource.cpp logic/sources/LogicStateSource.h logic/events/LogicStateEvent.h logic/chain/ChainEvent.h logic/LogicBuilder.cpp logic/LogicBuilder.h logic/events/EventWrapper.h logic/evaluators/CommonEvaluators.cpp logic/evaluators/CommonEvaluators.h logic/actions/CommonActions.cpp logic/actions/CommonActions.h logic/common/ForeachContext.h logic/conditions/TimeConditions.cpp logic/conditions/TimeConditions.h logic/chain/ChainContext.cpp logic/chain/ChainContext.h logic/chain/ChainDefinitions.h logic/chain/ChainEvaluators.cpp logic/chain/ChainEvaluators.h logic/chain/InstanceGenerator.cpp logic/chain/InstanceGenerator.h logic/chain/ChainLock.cpp logic/chain/ChainLock.h logic/chain/GlobalChainContext.cpp logic/chain/GlobalChainContext.h logic/sources/TriggerSource.cpp logic/sources/TriggerSource.h logic/events/TriggerEvent.cpp logic/events/TriggerEvent.h logic/conditions/TriggerConditions.cpp logic/conditions/TriggerConditions.h logic/chain/ChainGroup.cpp logic/chain/ChainGroup.h logic/chain/LockConfiguration.cpp logic/chain/LockConfiguration.h logic/evaluators/TriggerEvaluators.cpp logic/evaluators/TriggerEvaluators.h logic/context/LogicContext.cpp logic/context/LogicContext.h
        logic/state/DeferredNotify.h logic/state/DirectNotify.h logic/state/StateObserver.h)


add_library(logic SHARED ${SOURCE_FILES})
target_include_directories(logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE SYSTEM spdlog::spdlog)
target_link_libraries(logic PUBLIC core PRIVATE ${LIBS} spdlog::spdlog)
install(TARGETS logic COMPONENT logic)

add_subdirectory(test)