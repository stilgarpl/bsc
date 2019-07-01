#cmake_minimum_required(VERSION 3.7)
#project(basyco)

#set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread -lstdc++fs")

#set(CMAKE_LIBRARY_PATH  ${CMAKE_LIBRARY_PATH} "C:/_lib/poco-1.7.8p3")


find_package(Poco REQUIRED COMPONENTS  Net Util  Foundation REQUIRED)
find_package(Cereal CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
#FIND_PACKAGE(CPPexperimental)

set(SOURCE_FILES p2p/node/NodeInfo.cpp p2p/node/NodeInfo.h p2p/node/Node.cpp p2p/node/Node.h p2p/node/context/NodeContext.cpp p2p/node/context/NodeContext.h p2p/node/configuration/ConfigurationManager.cpp p2p/node/configuration/ConfigurationManager.h p2p/node/configuration/IConfig.h p2p/node/module/logic/ILogicModule.cpp p2p/node/module/logic/ILogicModule.h p2p/node/module/NodeModule.cpp p2p/node/module/NodeModule.h p2p/node/INode.h p2p/modules/filesystem/FilesystemModule.cpp p2p/modules/filesystem/FilesystemModule.h p2p/dependency/DependencyManaged.cpp p2p/dependency/DependencyManaged.h p2p/dependency/DependencyManager.cpp p2p/dependency/DependencyManager.h p2p/dependency/Dependency.cpp p2p/dependency/Dependency.h p2p/dependency/IDependency.h p2p/dependency/IDependencyManaged.h p2p/role/Role.cpp p2p/role/Role.h p2p/role/Roles.cpp p2p/role/Roles.h p2p/role/RoleInvalidException.cpp p2p/role/RoleInvalidException.h p2p/role/RoleScope.cpp p2p/role/RoleScope.h p2p/node/module/INodeModule.h p2p/modules/filesystem/identification/ResourceIdentificator.cpp p2p/modules/filesystem/identification/ResourceIdentificator.h p2p/modules/filesystem/identification/SimplePathRI.cpp p2p/modules/filesystem/identification/SimplePathRI.h p2p/modules/filesystem/network/packet/BeginTransfer.cpp p2p/modules/filesystem/network/packet/BeginTransfer.h p2p/modules/filesystem/network/packet/FinishTransfer.cpp p2p/modules/filesystem/network/packet/FinishTransfer.h p2p/modules/filesystem/identification/TransferTypes.h p2p/modules/filesystem/network/logic/events/TransferEvent.cpp p2p/modules/filesystem/network/logic/events/TransferEvent.h p2p/modules/filesystem/network/logic/sources/TransferSource.cpp p2p/modules/filesystem/network/logic/sources/TransferSource.h p2p/modules/filesystem/transfer/TransferManager.cpp p2p/modules/filesystem/transfer/TransferManager.h p2p/modules/filesystem/network/packet/DataTransfer.cpp p2p/modules/filesystem/network/packet/DataTransfer.h p2p/modules/filesystem/network/packet/PropertiesTransfer.cpp p2p/modules/filesystem/network/packet/PropertiesTransfer.h p2p/role/RoleDefinitions.cpp p2p/role/RoleDefinitions.h p2p/node/module/ModuleState.h p2p/modules/filesystem/data/FileData.cpp p2p/modules/filesystem/data/FileData.h p2p/modules/filesystem/network/packet/TransferQuery.cpp p2p/modules/filesystem/network/packet/TransferQuery.h p2p/modules/filesystem/network/processor/NetworkProcessors.cpp p2p/modules/filesystem/network/processor/NetworkProcessors.h p2p/node/module/INodeModule.cpp p2p/node/configuration/DataFileNotFoundException.cpp p2p/node/configuration/DataFileNotFoundException.h p2p/modules/filesystem/transfer/TransferException.cpp p2p/modules/filesystem/transfer/TransferException.h p2p/node/module/logic/ModuleSource.cpp p2p/node/module/logic/ModuleSource.h p2p/node/module/logic/events/ModuleEvent.cpp p2p/node/module/logic/events/ModuleEvent.h p2p/dummy.cpp)


add_library(p2p_core SHARED ${SOURCE_FILES})
add_library(p2p SHARED p2p/dummy.cpp)
target_include_directories(p2p_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE SYSTEM spdlog::spdlog ${Poco_INCLUDE_DIRS} cereal::cereal)
message(CURRENT SOURCE: ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(p2p_core logic ${Poco_LIBRARIES} ${LIBS} spdlog::spdlog)
install(TARGETS p2p_core COMPONENT p2p_core)

add_subdirectory(test)

add_subdirectory(p2p/modules)

target_link_libraries(p2p PUBLIC p2p_core logic auth basic filesystem command PRIVATE ${Poco_LIBRARIES} ${LIBS} spdlog::spdlog)