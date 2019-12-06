#setDirect(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_logic/test_main.cpp test_logic/logic_test.cpp)

add_executable(test_logic ${SOURCE_FILES})
catch_discover_tests(test_logic)

target_link_libraries(test_logic logic Catch2::Catch2 ${LIBS})
