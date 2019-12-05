find_package(spdlog CONFIG REQUIRED)

set(SOURCE_FILES core/thread/Runnable.cpp core/thread/Runnable.h core/context/Context.h core/signal/Signal.h core/uber/Uber.h core/utils/cereal_include.h core/context/Context.cpp core/utils/from_string.h core/utils/template_cast.h core/signal/Executor.cpp core/signal/Executor.h core/utils/crypto.h core/utils/crypto.cpp core/signal/OrderedExecutionPolicy.h core/log/Logger.cpp core/log/Logger.h core/log/LoggerContext.cpp core/log/LoggerContext.h core/signal/DefaultExecutionPolicy.cpp core/signal/DefaultExecutionPolicy.h core/signal/OrderedExecutor.cpp core/signal/OrderedExecutor.h core/signal/SimpleExecutor.cpp core/signal/SimpleExecutor.h core/signal/Executor.cpp core/signal/Executor.h core/signal/ThreadExecutor.cpp core/signal/ThreadExecutor.h core/signal/ThreadPoolExecutor.cpp core/signal/ThreadPoolExecutor.h core/signal/ExecutorPolicyTraits.h core/factory/Factory.cpp core/factory/Factory.h core/factory/FactoryContext.cpp core/factory/FactoryContext.h core/thread/ThreadQueueProcessor.cpp core/thread/ThreadQueueProcessor.h core/io/InputOutputContext.cpp core/io/InputOutputContext.h)

add_library(core ${SOURCE_FILES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE SYSTEM spdlog::spdlog)
target_link_libraries(core PRIVATE ${LIBS} spdlog::spdlog)
install(TARGETS core COMPONENT core)

add_subdirectory(test)