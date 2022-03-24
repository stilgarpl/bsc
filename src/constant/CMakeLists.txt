set(SOURCE_FILES constant/standard/World.h constant/dummy.cpp ../math/math/arithmetic.h)

add_library(constant ${SOURCE_FILES})
set_target_properties(constant PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(constant PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
if (BSC_USE_PCH)
    target_precompile_headers(constant PRIVATE <string> <filesystem> <map> <memory> <iostream> <mutex>
            <constant/constant/constant.h>
            )
endif ()
install(DIRECTORY constant DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
target_link_libraries(constant PUBLIC)
install(TARGETS constant COMPONENT constant DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()