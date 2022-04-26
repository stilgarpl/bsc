set(SOURCE_FILES optional/optional.h optional/dummy.cpp )

add_library(optional ${SOURCE_FILES})
set_target_properties(optional PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(optional PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
if (BSC_USE_PCH)
    target_precompile_headers(optional PRIVATE <string> <filesystem> <map> <memory> <iostream> <mutex>
            <optional/optional/optional.h>
            )
endif ()
install(DIRECTORY optional DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
target_link_libraries(optional PUBLIC)
install(TARGETS optional COMPONENT optional DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()