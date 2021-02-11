set(SOURCE_FILES test_log/test_main.cpp test_log/test_log.cpp)

add_executable(test_log ${SOURCE_FILES})
catch_discover_tests(test_log WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_log PUBLIC test_log)
target_link_libraries(test_log PUBLIC log PRIVATE testaid)
