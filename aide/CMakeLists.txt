
set(SOURCE_FILES aide/Aide.cpp aide/Aide.h)

add_library(aide ${SOURCE_FILES})
target_include_directories(aide PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_compile_options(aide PUBLIC)
#target_precompile_headers(aide PRIVATE)
#target_link_libraries(aide PUBLIC )
install(DIRECTORY aide DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
install(TARGETS aide COMPONENT aide DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)

if (${BSC_ENABLE_TESTS})
    add_subdirectory(test)
endif ()