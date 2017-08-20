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
set(SOURCE_FILES main.cpp network/node/NodeInfo.cpp network/node/NodeInfo.h network/node/NetworkInfo.cpp network/node/NetworkInfo.h network/node/Node.cpp network/node/Node.h network/protocol/connection/ServerConnection.cpp network/protocol/connection/ServerConnection.h network/protocol/packet/NetworkPacket.cpp network/protocol/packet/NetworkPacket.h network/protocol/connection/ClientConnection.cpp network/protocol/connection/ClientConnection.h network/protocol/logic/ServerLogic.cpp network/protocol/logic/ServerLogic.h network/protocol/logic/IServerLogic.h network/protocol/enum/Command.h network/protocol/logic/IClientLogic.h network/protocol/logic/ClientLogic.cpp network/protocol/logic/ClientLogic.h network/service/NetworkServiceManager.cpp network/service/NetworkServiceManager.h network/protocol/connection/Connection.cpp network/protocol/connection/Connection.h network/protocol/processor/ConnectionProcessor.cpp network/protocol/processor/ConnectionProcessor.h thread/Runnable.cpp thread/Runnable.h network/protocol/context/Context.h)
add_executable(basyco ${SOURCE_FILES})
target_link_libraries(basyco ${LIBS})

