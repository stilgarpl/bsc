#set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES)

add_executable(test_logic ${SOURCE_FILES})
add_test(NAME test_logic COMMAND test_logic)

target_link_libraries(test_logic p2p ${LIBS})
