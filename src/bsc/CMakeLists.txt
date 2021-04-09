add_executable(bsc-demo bsc/main_demo.cpp)
add_executable(bsc-control bsc/main_control.cpp)
add_executable(bsc-daemon bsc/main_daemon.cpp)
add_executable(bsc-test bsc/main_test.cpp)

target_link_libraries(bsc-demo repo module_auth)
target_link_libraries(bsc-control p2p_core module_command)
target_link_libraries(bsc-daemon repo)
target_link_libraries(bsc-test properties context log)
install(TARGETS bsc-demo bsc-daemon bsc-control bsc-test COMPONENT bsc)

