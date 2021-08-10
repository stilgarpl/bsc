set(SOURCE_FILES test_context/auto_test.cpp test_context/context_test.cpp test_context/test_main.cpp test_context/flags_test.cpp)

add_executable(test_context ${SOURCE_FILES})
catch_discover_tests(test_context WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_context PUBLIC test_context)
target_link_libraries(test_context PUBLIC context PRIVATE testaid)
