set(SOURCE_FILES test_logic/test_main.cpp test_logic/logic_test.cpp test_logic/state_machine_test.cpp)

add_executable(test_logic ${SOURCE_FILES})
catch_discover_tests(test_logic WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(test_logic logic Catch2::Catch2 testaid)
