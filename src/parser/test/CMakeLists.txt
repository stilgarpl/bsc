#setDirect(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_parser/test_main.cpp test_parser/fromString_test.cpp test_parser/templateCast_test.cpp test_parser/parameters_test.cpp)

add_executable(test_parser ${SOURCE_FILES})
catch_discover_tests(test_parser WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_parser PUBLIC test_parser)
target_link_libraries(test_parser PUBLIC parser)
