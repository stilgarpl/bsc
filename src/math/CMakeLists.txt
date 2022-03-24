set(SOURCE_FILES math/arithmetic.h math/dummy.cpp)

add_library(math ${SOURCE_FILES})
set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(math PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
if (BSC_USE_PCH)
    target_precompile_headers(math PRIVATE <string> <filesystem> <map> <memory> <iostream> <mutex>
            <constant/constant/constant.h>
            )
endif ()
install(DIRECTORY math DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
target_link_libraries(math PUBLIC)
install(TARGETS math COMPONENT math DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()