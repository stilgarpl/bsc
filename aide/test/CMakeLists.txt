#setDirect(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_aide/test_main.cpp test_aide/aide_test.cpp)

add_executable(test_aide ${SOURCE_FILES})
catch_discover_tests(test_aide WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_aide PUBLIC test_aide)
target_link_libraries(test_aide PUBLIC aide)
