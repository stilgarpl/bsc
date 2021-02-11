add_executable(file-sorter main_file_sorter.cpp)

target_link_libraries(file-sorter io)
install(TARGETS bsc-demo bsc-daemon bsc-control bsc-test COMPONENT bsc)

