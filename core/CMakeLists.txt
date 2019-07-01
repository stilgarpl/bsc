find_package(spdlog CONFIG REQUIRED)

set(SOURCE_FILES core/thread/Runnable.cpp core/thread/Runnable.h core/context/Context.h core/signal/Signal.h core/uber/Uber.h core/utils/cereal_include.h core/context/Context.cpp core/utils/from_string.h core/utils/template_cast.h core/signal/Executor.cpp core/signal/Executor.h core/utils/crypto.h core/utils/crypto.cpp core/signal/ExecutionPolicy.cpp core/signal/ExecutionPolicy.h core/signal/OrderedExecutionPolicy.cpp core/signal/OrderedExecutionPolicy.h core/log/Logger.cpp core/log/Logger.h core/log/LoggerContext.cpp core/log/LoggerContext.h)

add_library(core SHARED ${SOURCE_FILES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE SYSTEM spdlog::spdlog)
target_link_libraries(core PRIVATE ${LIBS} spdlog::spdlog)
install(TARGETS core COMPONENT core)

add_subdirectory(test)