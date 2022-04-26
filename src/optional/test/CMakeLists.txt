set(SOURCE_FILES test_optional/test_main.cpp)

add_executable(test_optional ${SOURCE_FILES})
catch_discover_tests(test_optional WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_math PUBLIC test_math)
target_link_libraries(test_optional PUBLIC optional PRIVATE testaid)
