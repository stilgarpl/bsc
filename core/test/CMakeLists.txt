#setDirect(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_core/test_main.cpp test_core/signal_test.cpp test_core/context_test.cpp test_core/uber_test.cpp test_core/queue_processor_test.cpp test_core/sha1_test.cpp test_core/getset_test.cpp test_core/generator_test.cpp test_core/cereal_test.cpp)

add_executable(test_core ${SOURCE_FILES})
catch_discover_tests(test_core WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_core PUBLIC test_core)
target_link_libraries(test_core PUBLIC core PRIVATE tester)
