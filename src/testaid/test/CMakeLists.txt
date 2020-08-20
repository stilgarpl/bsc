set(SOURCE_FILES test_testaid/test_main.cpp test_testaid/testaid_test.cpp)

add_executable(test_testaid ${SOURCE_FILES})
catch_discover_tests(test_testaid WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_testaid PUBLIC test_testaid)
target_link_libraries(test_testaid PUBLIC testaid)
