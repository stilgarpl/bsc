#setDirect(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_repo/test_main.cpp test_repo/journal_test.cpp test_repo/repo_test.cpp test_repo/repo_map_renderer_test.cpp)

add_executable(test_repo ${SOURCE_FILES})
catch_discover_tests(test_repo WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_repo PUBLIC test_repo)
target_link_libraries(test_repo repo tester)
