find_package(spdlog REQUIRED)
set(SOURCE_FILES log/log/Logger.cpp log/log/Logger.h log/log/LoggerContext.cpp log/log/LoggerContext.h)

add_library(log ${SOURCE_FILES})
target_include_directories(log PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
if (BSC_USE_PCH)
    target_precompile_headers(log PRIVATE
            <filesystem>
            )
endif ()
install(DIRECTORY log DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
target_link_libraries(log PUBLIC context spdlog::spdlog)
install(TARGETS log COMPONENT log DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()