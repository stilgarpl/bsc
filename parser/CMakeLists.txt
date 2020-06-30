
set(SOURCE_FILES parser/parameters/CommandLineParameters.cpp parser/parameters/CommandLineParameters.h parser/parser/fromString.h parser/cast/templateCast.h parser/parser/explode.h)

add_library(parser ${SOURCE_FILES})
target_include_directories(parser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_compile_options(parser PUBLIC -fconcepts)
if (BSC_USE_PCH)
    target_precompile_headers(parser PRIVATE
            <parser/parameters/CommandLineParameters.h>
            <parser/parser/fromString.h>
            <parser/cast/templateCast.h>
            <parser/parser/explode.h>)
endif ()
#target_link_libraries(parser PUBLIC )
install(DIRECTORY parser DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
install(TARGETS parser COMPONENT parser DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)

if (${BSC_ENABLE_TESTS})
    add_subdirectory(test)
endif ()