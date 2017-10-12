cmake_minimum_required(VERSION 3.7)
project(basyco)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread -lstdc++fs")

add_subdirectory(p2p)
add_subdirectory(basyco)
