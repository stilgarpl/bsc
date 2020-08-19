#setDirect(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_io/test_main.cpp test_io/file_type_test.cpp test_io/file_sorter_test.cpp)

add_executable(test_io ${SOURCE_FILES})
catch_discover_tests(test_io WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_io PUBLIC test_io)
target_link_libraries(test_io PUBLIC io PRIVATE testaid)
