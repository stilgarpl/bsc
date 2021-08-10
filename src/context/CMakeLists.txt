set(SOURCE_FILES context/auto/AutoCaller.h context/context/Context.cpp context/registry/InitializerRegistry.h context/registry/Registry.h context/options/Flags.h)

add_library(context ${SOURCE_FILES})
target_include_directories(context PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
if (BSC_USE_PCH)
    target_precompile_headers(context PRIVATE <string> <filesystem> <map> <memory> <iostream> <mutex>
            <context/context/Context.h>
            )
endif ()
install(DIRECTORY context DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
target_link_libraries(context PUBLIC)
install(TARGETS context COMPONENT context DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()