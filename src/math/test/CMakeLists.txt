set(SOURCE_FILES test_math/math_test.cpp test_math/test_main.cpp)

add_executable(test_math ${SOURCE_FILES})
catch_discover_tests(test_math WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

#target_include_directories(test_math PUBLIC test_math)
target_link_libraries(test_math PUBLIC math PRIVATE testaid)
