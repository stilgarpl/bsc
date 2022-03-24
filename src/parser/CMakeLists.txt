find_package(argp REQUIRED)
find_package(magic_enum REQUIRED)
find_package(fmt REQUIRED)
set(SOURCE_FILES parser/parameters/CommandLineParameters.cpp parser/parameters/CommandLineParameters.h parser/parser/fromString.h parser/cast/templateCast.h parser/parser/explode.h parser/writer/toString.h parser/configuration/ParserConfiguration.h parser/concepts/concepts.h parser/value/ParametrizedValue.cpp parser/value/ParametrizedValue.h parser/value/StandardAffixes.h parser/value/Affix.h)

add_library(parser ${SOURCE_FILES})
target_include_directories(parser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)

if (BSC_USE_PCH)
    target_precompile_headers(parser PRIVATE
            <parser/parameters/CommandLineParameters.h>
            <parser/parser/fromString.h>
            <parser/cast/templateCast.h>
            <parser/parser/explode.h>)
endif ()
target_link_libraries(parser PUBLIC argp::argp fmt::fmt-header-only constant math PRIVATE magic_enum::magic_enum  )
install(DIRECTORY parser DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
install(TARGETS parser COMPONENT parser DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)

if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()