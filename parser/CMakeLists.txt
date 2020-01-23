
set(SOURCE_FILES parser/parameters/CommandLineParameters.cpp parser/parameters/CommandLineParameters.h parser/parser/fromString.h parser/cast/templateCast.h parser/parser/explode.h)
set(HEADER_FILES parser/parameters/CommandLineParameters.h parser/parser/fromString.h parser/cast/templateCast.h parser/parser/explode.h)

add_library(parser ${SOURCE_FILES})
target_include_directories(parser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_compile_options(parser PUBLIC -fconcepts)
#target_link_libraries(parser PUBLIC )
install(DIRECTORY parser DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
install(TARGETS parser COMPONENT parser DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)

add_subdirectory(test)