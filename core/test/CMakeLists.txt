#set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test_core/test_main.cpp)

add_executable(test_core ${SOURCE_FILES})
add_test(NAME test_core COMMAND test_core)

#target_include_directories(test_core PUBLIC test_core)
target_link_libraries(test_core core ${LIBS})
