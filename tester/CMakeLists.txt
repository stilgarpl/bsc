
set(SOURCE_FILES tester/Tester.cpp tester/Tester.h)

add_library(tester ${SOURCE_FILES})
target_include_directories(tester PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_compile_options(tester PUBLIC)
#target_precompile_headers(tester PRIVATE)
#target_link_libraries(tester PUBLIC )
install(DIRECTORY tester DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
install(TARGETS tester COMPONENT tester DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)

if (${BSC_ENABLE_TESTS})
    add_subdirectory(test)
endif ()