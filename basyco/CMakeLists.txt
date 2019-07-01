#cmake_minimum_required(VERSION 3.7)
#project(basyco)

#set(CMAKE_CXX_STANDARD 17)

#FIND_PACKAGE(CPPexperimental)

#message(libs:: ${LIBS})
#
#
#message(includ:: ${INCLUDES})
#set(LIBS "${CPPEXPERIMENTAL_LIBRARIES} ${CONAN_LIBS}")
#
#message(libs:: ${LIBS})

set(SOURCE_FILES)
add_executable(basyco-demo basyco/main_demo.cpp)
add_executable(basyco-control basyco/main_control.cpp)
add_executable(basyco-daemon basyco/main_daemon.cpp)
#target_include_directories(basyco PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(basyco PUBLIC p2p)
target_link_libraries(basyco-demo repo auth)
target_link_libraries(basyco-control repo)
target_link_libraries(basyco-daemon repo)
install(TARGETS basyco-demo basyco-daemon basyco-control COMPONENT basyco)

