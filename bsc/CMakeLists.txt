#cmake_minimum_required(VERSION 3.7)
#project(bsc)

#setDirect(CMAKE_CXX_STANDARD 17)

#FIND_PACKAGE(CPPexperimental)

#message(libs:: ${LIBS})
#
#
#message(includ:: ${INCLUDES})
#setDirect(LIBS "${CPPEXPERIMENTAL_LIBRARIES} ${CONAN_LIBS}")
#
#message(libs:: ${LIBS})

set(SOURCE_FILES)
add_executable(bsc-demo bsc/main_demo.cpp)
add_executable(bsc-control bsc/main_control.cpp)
add_executable(file-sorter bsc/main_file_sorter.cpp)
add_executable(bsc-daemon bsc/main_daemon.cpp)

add_executable(bsc-test bsc/main_test.cpp)
#target_include_directories(bsc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(bsc PUBLIC p2p)
target_link_libraries(bsc-demo repo module_auth)
target_link_libraries(bsc-control p2p_core module_command)
target_link_libraries(bsc-daemon repo)
target_link_libraries(bsc-test core parser)
target_link_libraries(file-sorter io)
install(TARGETS bsc-demo bsc-daemon bsc-control bsc-test COMPONENT bsc)

