#setDirect(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_tester/test_main.cpp test_tester/tester_test.cpp)

add_executable(test_tester ${SOURCE_FILES})
catch_discover_tests(test_tester WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_tester PUBLIC test_tester)
target_link_libraries(test_tester PUBLIC tester)
