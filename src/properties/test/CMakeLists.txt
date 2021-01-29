set(SOURCE_FILES test_properties/test_main.cpp test_properties/test_property.cpp test_properties/test_yaml_parser.cpp)

add_executable(test_properties ${SOURCE_FILES})
catch_discover_tests(test_properties WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_properties PUBLIC test_properties)
target_link_libraries(test_properties PUBLIC properties testaid)
