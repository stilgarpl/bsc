find_package(Poco REQUIRED Crypto)
find_package(OpenSSL REQUIRED)
find_package(magic_enum REQUIRED)

set(SOURCE_FILES core/thread/Runnable.cpp core/thread/Runnable.h core/signal/Signal.h core/uber/Uber.h core/utils/cereal_include.h core/signal/Executor.h core/utils/crypto.h core/utils/crypto.cpp core/signal/OrderedExecutionPolicy.h core/signal/DefaultExecutionPolicy.h core/signal/OrderedExecutor.cpp core/signal/OrderedExecutor.h core/signal/SimpleExecutor.cpp core/signal/SimpleExecutor.h core/signal/Executor.h core/signal/ThreadExecutor.cpp core/signal/ThreadExecutor.h core/signal/ThreadPoolExecutor.cpp core/signal/ThreadPoolExecutor.h core/signal/ExecutorPolicyTraits.h core/factory/Factory.h core/factory/FactoryContext.h core/thread/ThreadQueueProcessor.h core/io/InputOutputContext.cpp core/io/InputOutputContext.h core/getset/GetSet.h core/generator/Generator.h core/generator/SequenceAlgorithm.h core/generator/RandomAlgorithm.h core/generator/RandomHashStringAlgorithm.cpp core/generator/RandomHashStringAlgorithm.h core/factory/CopyFactory.h core/factory/ParsingFactory.h core/factory/AutoFactory.h core/factory/FactoryPtr.h core/text/TextUtils.cpp core/text/TextUtils.h core/optional/optional.h core/factory/FactoryParameter.cpp core/factory/FactoryParameter.h)

add_library(core ${SOURCE_FILES})
target_include_directories(core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
if (BSC_USE_PCH)
    target_precompile_headers(core PRIVATE <string> <filesystem> <map> <memory> <iostream> <mutex>
            <core/thread/Runnable.h>
            <core/context/Context.h>
            <core/signal/Signal.h>
            <core/uber/Uber.h>
            <core/utils/cereal_include.h>
            <core/utils/crypto.h>
            <log/log/Logger.h>
            )
endif ()
install(DIRECTORY core DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
target_link_libraries(core PUBLIC log parser context Poco::Crypto PRIVATE magic_enum::magic_enum)
install(TARGETS core COMPONENT core DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()