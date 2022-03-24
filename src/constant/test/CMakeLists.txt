set(SOURCE_FILES test_constant/constant_test.cpp test_constant/test_main.cpp)

add_executable(test_constant ${SOURCE_FILES})
catch_discover_tests(test_constant WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_math PUBLIC test_math)
target_link_libraries(test_constant PUBLIC constant PRIVATE testaid)
