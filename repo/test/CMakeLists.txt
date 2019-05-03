#set(CMAKE_CXX_STANDARD 17)

enable_testing()

set(SOURCE_FILES test_repo/test_main.cpp test_repo/JournalTest.cpp test_repo/repo_test.cpp)

add_executable(test_repo ${SOURCE_FILES})
add_test(NAME test_repo COMMAND test_repo)

target_include_directories(test_repo PUBLIC test_repo)
target_link_libraries(test_repo p2p repo ${LIBS})
