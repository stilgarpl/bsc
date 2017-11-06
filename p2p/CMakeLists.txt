#cmake_minimum_required(VERSION 3.7)
#project(basyco)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread -lstdc++fs")

#set(CMAKE_LIBRARY_PATH  ${CMAKE_LIBRARY_PATH} "C:/_lib/poco-1.7.8p3")


find_package(Poco REQUIRED COMPONENTS Foundation Net Util REQUIRED)
find_package(Cereal REQUIRED)
FIND_PACKAGE(CPPexperimental)


message(libs:: ${LIBS})

set(INCLUDES ${Poco_INCLUDE_DIRS} ${Cereal_INCLUDE_DIRS})
message(includ:: ${INCLUDES})
include_directories(${INCLUDES})
set(LIBS ${LIBS} ${Poco_LIBRARIES} ${CPPEXPERIMENTAL_LIBRARIES})

message(libs:: ${LIBS})


set(SOURCE_FILES p2p/network/node/NodeInfo.cpp p2p/network/node/NodeInfo.h p2p/network/node/NetworkInfo.cpp p2p/network/node/NetworkInfo.h p2p/network/node/Node.cpp p2p/network/node/Node.h p2p/network/protocol/connection/ServerConnection.cpp p2p/network/protocol/connection/ServerConnection.h p2p/network/protocol/packet/BasePacket.cpp p2p/network/protocol/packet/BasePacket.h p2p/network/protocol/connection/ClientConnection.cpp p2p/network/protocol/connection/ClientConnection.h p2p/network/protocol/enum/Command.h p2p/network/protocol/connection/Connection.cpp p2p/network/protocol/connection/Connection.h p2p/network/protocol/processor/ConnectionProcessor.cpp p2p/network/protocol/processor/ConnectionProcessor.h p2p/thread/Runnable.cpp p2p/thread/Runnable.h p2p/context/Context.h p2p/network/protocol/packet/CommandPacket.cpp p2p/network/protocol/packet/CommandPacket.h p2p/network/protocol/enum/Status.h p2p/network/protocol/context/ConnectionContext.cpp p2p/network/protocol/context/ConnectionContext.h p2p/log/Logger.cpp p2p/log/Logger.h p2p/network/protocol/context/NodeContext.cpp p2p/network/protocol/context/NodeContext.h p2p/network/protocol/context/IContextSetup.h p2p/configuration/ConfigurationManager.cpp p2p/configuration/ConfigurationManager.h p2p/configuration/IConfig.h p2p/signal/Signal.h p2p/logic/ISource.cpp p2p/logic/ISource.h p2p/logic/IEvent.h p2p/uber/Uber.h p2p/logic/SourceManager.cpp p2p/logic/SourceManager.h p2p/logic/sources/ClockSource.cpp p2p/logic/sources/ClockSource.h p2p/logic/events/ClockEvent.cpp p2p/logic/events/ClockEvent.h p2p/logic/events/Tick.cpp p2p/logic/events/Tick.h p2p/logic/ActionManager.cpp p2p/logic/ActionManager.h p2p/logic/LogicManager.cpp p2p/logic/LogicManager.h p2p/network/protocol/logic/sources/AuthSource.cpp p2p/network/protocol/logic/sources/AuthSource.h p2p/network/protocol/context/LogicContext.cpp p2p/network/protocol/context/LogicContext.h p2p/network/protocol/packet/AuthPacket.cpp p2p/network/protocol/packet/AuthPacket.h p2p/logic/sources/EventQueueSource.h p2p/network/protocol/logic/events/AuthEvent.cpp p2p/network/protocol/logic/events/AuthEvent.h p2p/network/protocol/logic/sources/ConnectionSource.cpp p2p/network/protocol/logic/sources/ConnectionSource.h p2p/network/protocol/logic/events/ConnectionEvent.cpp p2p/network/protocol/logic/events/ConnectionEvent.h p2p/network/protocol/logic/actions/ProtocolActions.cpp p2p/network/protocol/logic/actions/ProtocolActions.h p2p/network/protocol/logic/events/PacketEvent.cpp p2p/network/protocol/logic/events/PacketEvent.h p2p/network/protocol/logic/actions/BasePacketInfo.cpp p2p/network/protocol/logic/actions/TransmissionControl.cpp p2p/network/protocol/logic/actions/TransmissionControl.h p2p/network/protocol/protocol/IProtocol.cpp p2p/network/protocol/protocol/IProtocol.h p2p/network/protocol/protocol/GravitonProtocol.cpp p2p/network/protocol/protocol/GravitonProtocol.h p2p/network/node/protocol/logic/events/NodeInfoEvent.cpp p2p/network/node/protocol/logic/events/NodeInfoEvent.h p2p/network/node/protocol/logic/sources/NodeSource.cpp p2p/network/node/protocol/logic/sources/NodeSource.h p2p/network/node/protocol/packet/NodeInfoRequest.cpp p2p/network/node/protocol/packet/NodeInfoRequest.h p2p/network/node/protocol/packet/NodeInfoResponse.cpp p2p/network/node/protocol/packet/NodeInfoResponse.h p2p/utils/cereal_include.h p2p/network/node/protocol/logic/actions/NodeActions.cpp p2p/network/node/protocol/logic/actions/NodeActions.h p2p/context/Context.cpp p2p/network/node/protocol/packet/NetworkInfoRequest.cpp p2p/network/node/protocol/packet/NetworkInfoRequest.h p2p/network/node/protocol/packet/NetworkInfoResponse.cpp p2p/network/node/protocol/packet/NetworkInfoResponse.h p2p/network/node/protocol/logic/events/NetworkInfoEvent.cpp p2p/network/node/protocol/logic/events/NetworkInfoEvent.h p2p/network/node/protocol/logic/sources/NetworkSource.cpp p2p/network/node/protocol/logic/sources/NetworkSource.h p2p/network/node/protocol/logic/actions/NetworkActions.cpp p2p/network/node/protocol/logic/actions/NetworkActions.h p2p/network/protocol/packet/info/PacketInfo.cpp p2p/network/protocol/packet/info/PacketInfo.h p2p/network/node/protocol/packet/NodeInfoGroup.cpp p2p/network/node/protocol/packet/NodeInfoGroup.h p2p/network/node/protocol/packet/NetworkInfoGroup.h p2p/network/protocol/connection/IServerConnection.h p2p/filesystem/network/packet/SendFile.cpp p2p/filesystem/network/packet/SendFile.h p2p/filesystem/network/logic/sources/FileSource.cpp p2p/filesystem/network/logic/sources/FileSource.h p2p/filesystem/network/logic/events/FileRequestEvent.cpp p2p/filesystem/network/logic/events/FileRequestEvent.h p2p/filesystem/network/logic/actions/FileActions.cpp p2p/filesystem/network/logic/actions/FileActions.h p2p/filesystem/network/logic/events/FileResponseEvent.cpp p2p/filesystem/network/logic/events/FileResponseEvent.h p2p/network/protocol/context/ProcessorContext.cpp p2p/network/protocol/context/ProcessorContext.h p2p/filesystem/network/logic/events/FileAttributesEvent.cpp p2p/filesystem/network/logic/events/FileAttributesEvent.h p2p/logic/ILogicModule.cpp p2p/logic/ILogicModule.h p2p/network/node/NodeModule.cpp p2p/network/node/NodeModule.h p2p/network/node/INode.h p2p/network/logic/events/NetworkingEvent.h)
add_library(p2p SHARED ${SOURCE_FILES})
target_include_directories(p2p PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
message(CURRENT SOURCE: ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(p2p ${LIBS})

