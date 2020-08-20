find_package(spdlog CONFIG REQUIRED)

set(SOURCE_FILES logic/ISource.cpp logic/ISource.h logic/IEvent.h logic/SourceManager.cpp logic/SourceManager.h logic/sources/ClockSource.cpp logic/sources/ClockSource.h logic/events/Tick.cpp logic/events/Tick.h logic/ActionManager.cpp logic/ActionManager.h logic/LogicManager.cpp logic/LogicManager.h logic/sources/EventQueueSource.h logic/sources/AutoSource.cpp logic/sources/AutoSource.h logic/LogicObject.cpp logic/LogicObject.h logic/state/StateMachine.cpp logic/state/StateMachine.h logic/state/LogicStateMachine.h logic/sources/LogicStateSource.cpp logic/sources/LogicStateSource.h logic/events/LogicStateEvent.h logic/chain/ChainEvent.h logic/LogicBuilder.cpp logic/LogicBuilder.h logic/events/EventWrapper.h logic/evaluators/CommonEvaluators.cpp logic/evaluators/CommonEvaluators.h logic/actions/CommonActions.h logic/common/ForeachContext.h logic/conditions/TimeConditions.h logic/chain/ChainContext.cpp logic/chain/ChainContext.h logic/chain/ChainDefinitions.h logic/chain/ChainEvaluators.h logic/chain/InstanceGenerator.cpp logic/chain/InstanceGenerator.h logic/chain/ChainLock.cpp logic/chain/ChainLock.h logic/chain/GlobalChainContext.h logic/sources/TriggerSource.cpp logic/sources/TriggerSource.h logic/events/TriggerEvent.h logic/conditions/TriggerConditions.h logic/chain/ChainGroup.cpp logic/chain/ChainGroup.h logic/chain/LockConfiguration.h logic/evaluators/TriggerEvaluators.cpp logic/evaluators/TriggerEvaluators.h logic/context/LogicContext.cpp logic/context/LogicContext.h
        logic/state/DeferredNotify.h logic/state/DirectNotify.h logic/state/StateObserver.h logic/LogicExceptions.cpp logic/LogicExceptions.h)


add_library(logic ${SOURCE_FILES})
target_include_directories(logic PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
if (BSC_USE_PCH)
    target_precompile_headers(logic REUSE_FROM core)
endif ()
target_link_libraries(logic PUBLIC core PRIVATE)
install(DIRECTORY logic DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
install(TARGETS logic COMPONENT logic DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()