
set(SOURCE_FILES testaid/testaid.cpp testaid/testaid.h)

add_library(testaid ${SOURCE_FILES})
target_include_directories(testaid PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_compile_options(testaid PUBLIC)
#target_precompile_headers(testaid PRIVATE)
#target_link_libraries(testaid PUBLIC )
install(DIRECTORY testaid DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
install(TARGETS testaid COMPONENT testaid DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)

if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()