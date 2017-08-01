cmake_minimum_required(VERSION 3.7)
project(basyco)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Poco REQUIRED COMPONENTS Foundation Net Util REQUIRED)
find_package(Cereal REQUIRED)

message(libs:: ${LIBS})
message(libs:: ${LIBS})


include_directories(${Poco_INCLUDE_DIRS})
include_directories(${Cereal_INCLUDE_DIRS})
#include_directories(${Boost_LIBRARIES})
set(LIBS ${LIBS} ${Poco_LIBRARIES})

message(libs:: ${LIBS})
set(SOURCE_FILES main.cpp network/node/NodeInfo.cpp network/node/NodeInfo.h network/NetworkInfo.cpp network/NetworkInfo.h network/node/Node.cpp network/node/Node.h network/ServerConnection.cpp network/ServerConnection.h network/protocol/NetworkPacket.cpp network/protocol/NetworkPacket.h network/ClientConnection.cpp network/ClientConnection.h network/protocol/ServerLogic.cpp network/protocol/ServerLogic.h network/protocol/IServerLogic.h network/protocol/enum/Command.h)
add_executable(basyco ${SOURCE_FILES})
target_link_libraries(basyco ${LIBS})

