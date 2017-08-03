cmake_minimum_required(VERSION 3.7)
project(basyco)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread")

find_package(Poco REQUIRED COMPONENTS Foundation Net Util REQUIRED)
find_package(Cereal REQUIRED)

message(libs:: ${LIBS})

set(INCLUDES ${Poco_INCLUDE_DIRS} ${Cereal_INCLUDE_DIRS})
message(includ:: ${INCLUDES})
include_directories(${INCLUDES})
#include_directories()
#include_directories(${Boost_LIBRARIES})
set(LIBS ${LIBS} ${Poco_LIBRARIES})

message(libs:: ${LIBS})
set(SOURCE_FILES main.cpp network/node/NodeInfo.cpp network/node/NodeInfo.h network/node/NetworkInfo.cpp network/node/NetworkInfo.h network/node/Node.cpp network/node/Node.h network/protocol/ServerConnection.cpp network/protocol/ServerConnection.h network/protocol/NetworkPacket.cpp network/protocol/NetworkPacket.h network/protocol/ClientConnection.cpp network/protocol/ClientConnection.h network/protocol/logic/ServerLogic.cpp network/protocol/logic/ServerLogic.h network/protocol/logic/IServerLogic.h network/protocol/enum/Command.h network/protocol/logic/IClientLogic.h network/protocol/logic/ClientLogic.cpp network/protocol/logic/ClientLogic.h network/service/NetworkServiceManager.cpp network/service/NetworkServiceManager.h network/protocol/Connection.cpp network/protocol/Connection.h)
add_executable(basyco ${SOURCE_FILES})
target_link_libraries(basyco ${LIBS})

