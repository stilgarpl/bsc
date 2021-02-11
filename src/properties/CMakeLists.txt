find_package(yaml-cpp REQUIRED)
set(SOURCE_FILES properties/Property.h properties/PropertyTextLoader.cpp properties/PropertyTextLoader.h properties/PropertyContext.cpp properties/PropertyContext.h properties/PropertyDefinitions.h properties/parser/PropertyParser.cpp properties/parser/PropertyParser.h properties/parser/YamlParser.cpp properties/parser/YamlParser.h properties/parser/PropertyParserNodeType.h properties/PropertyExceptions.h properties/parser/PropertyParserController.cpp properties/parser/PropertyParserController.h properties/PropertyPrefix.h properties/PropertyFileLoader.cpp properties/PropertyFileLoader.h properties/PropertyLoader.cpp properties/PropertyLoader.h)

add_library(properties ${SOURCE_FILES})
target_include_directories(properties PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
#target_compile_options(properties PUBLIC -fconcepts)
if (BSC_USE_PCH)

endif ()
target_link_libraries(properties PUBLIC context parser yaml-cpp::yaml-cpp)
install(DIRECTORY properties DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
install(TARGETS properties COMPONENT properties DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)

if (BSC_ENABLE_TESTS)
    add_subdirectory(test)
endif ()