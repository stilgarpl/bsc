#setDirect(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_p2p/test_main.cpp test_p2p/transfer_test.cpp test_p2p/remote_node_test.cpp test_p2p/basic_module_test.cpp test_p2p/command_module_test.cpp)

add_executable(test_p2p ${SOURCE_FILES})
catch_discover_tests(test_p2p WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_p2p PUBLIC test_p2p)
target_link_libraries(test_p2p p2p logic aide)
