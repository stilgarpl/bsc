cmake_minimum_required(VERSION 3.7)
project(basyco)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread -lstdc++fs")

set(CMAKE_LIBRARY_PATH  ${CMAKE_LIBRARY_PATH} "C:/_lib/poco-1.7.8p3")

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
set(SOURCE_FILES main.cpp network/node/NodeInfo.cpp network/node/NodeInfo.h network/node/NetworkInfo.cpp network/node/NetworkInfo.h network/node/Node.cpp network/node/Node.h network/protocol/connection/ServerConnection.cpp network/protocol/connection/ServerConnection.h network/protocol/packet/BasePacket.cpp network/protocol/packet/BasePacket.h network/protocol/connection/ClientConnection.cpp network/protocol/connection/ClientConnection.h network/protocol/enum/Command.h network/service/NetworkServiceManager.cpp network/service/NetworkServiceManager.h network/protocol/connection/Connection.cpp network/protocol/connection/Connection.h network/protocol/processor/ConnectionProcessor.cpp network/protocol/processor/ConnectionProcessor.h thread/Runnable.cpp thread/Runnable.h context/Context.h network/protocol/packet/CommandPacket.cpp network/protocol/packet/CommandPacket.h network/protocol/enum/Status.h network/protocol/context/ConnectionContext.cpp network/protocol/context/ConnectionContext.h log/Logger.cpp log/Logger.h network/protocol/context/NodeContext.cpp network/protocol/context/NodeContext.h network/protocol/context/IContextSetup.h configuration/ConfigurationManager.cpp configuration/ConfigurationManager.h configuration/IConfig.h signal/Signal.h logic/ISource.cpp logic/ISource.h logic/IEvent.h uber/Uber.h logic/SourceManager.cpp logic/SourceManager.h logic/sources/ClockSource.cpp logic/sources/ClockSource.h logic/events/ClockEvent.cpp logic/events/ClockEvent.h logic/events/Tick.cpp logic/events/Tick.h logic/ActionManager.cpp logic/ActionManager.h logic/LogicManager.cpp logic/LogicManager.h network/logic/events/NetEvent.cpp network/logic/events/NetEvent.h network/protocol/logic/sources/AuthSource.cpp network/protocol/logic/sources/AuthSource.h network/protocol/context/LogicContext.cpp network/protocol/context/LogicContext.h network/protocol/packet/AuthPacket.cpp network/protocol/packet/AuthPacket.h logic/sources/EventQueueSource.h network/protocol/logic/events/AuthEvent.cpp network/protocol/logic/events/AuthEvent.h network/protocol/logic/sources/ConnectionSource.cpp network/protocol/logic/sources/ConnectionSource.h network/protocol/logic/events/ConnectionEvent.cpp network/protocol/logic/events/ConnectionEvent.h network/protocol/logic/actions/ProtocolActions.cpp network/protocol/logic/actions/ProtocolActions.h network/protocol/logic/events/PacketEvent.cpp network/protocol/logic/events/PacketEvent.h network/protocol/logic/actions/NetworkPacketInfo.cpp network/protocol/logic/actions/TransmissionControl.cpp network/protocol/logic/actions/TransmissionControl.h network/protocol/protocol/IProtocol.cpp network/protocol/protocol/IProtocol.h network/protocol/protocol/GravitonProtocol.cpp network/protocol/protocol/GravitonProtocol.h network/node/protocol/logic/events/NodeInfoEvent.cpp network/node/protocol/logic/events/NodeInfoEvent.h network/node/protocol/logic/sources/NodeSource.cpp network/node/protocol/logic/sources/NodeSource.h network/node/protocol/packet/NodeInfoRequest.cpp network/node/protocol/packet/NodeInfoRequest.h network/node/protocol/packet/NodeInfoResponse.cpp network/node/protocol/packet/NodeInfoResponse.h utils/cereal_include.h network/node/protocol/logic/actions/NodeActions.cpp network/node/protocol/logic/actions/NodeActions.h context/Context.cpp network/node/protocol/packet/NetworkInfoRequest.cpp network/node/protocol/packet/NetworkInfoRequest.h network/node/protocol/packet/NetworkInfoResponse.cpp network/node/protocol/packet/NetworkInfoResponse.h network/node/protocol/logic/events/NetworkInfoEvent.cpp network/node/protocol/logic/events/NetworkInfoEvent.h network/node/protocol/logic/sources/NetworkSource.cpp network/node/protocol/logic/sources/NetworkSource.h network/node/protocol/logic/actions/NetworkActions.cpp network/node/protocol/logic/actions/NetworkActions.h)
add_executable(basyco ${SOURCE_FILES})
target_link_libraries(basyco ${LIBS})

