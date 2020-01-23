
find_package(Poco REQUIRED COMPONENTS Net Util Foundation REQUIRED)
find_package(Cereal CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(SOURCE_FILES p2p/node/NodeInfo.cpp p2p/node/NodeInfo.h p2p/node/Node.cpp p2p/node/Node.h p2p/node/context/NodeContext.cpp p2p/node/context/NodeContext.h p2p/node/configuration/ConfigurationManager.cpp p2p/node/configuration/ConfigurationManager.h p2p/node/configuration/IConfig.h p2p/node/module/logic/ILogicModule.cpp p2p/node/module/logic/ILogicModule.h p2p/node/module/NodeModule.cpp p2p/node/module/NodeModule.h p2p/node/INode.h p2p/dependency/DependencyManaged.h p2p/dependency/DependencyManager.h p2p/dependency/Dependency.h p2p/dependency/IDependency.h p2p/dependency/IDependencyManaged.h p2p/role/Role.cpp p2p/role/Role.h p2p/role/Roles.cpp p2p/role/Roles.h p2p/role/RoleInvalidException.cpp p2p/role/RoleInvalidException.h p2p/role/RoleScope.cpp p2p/role/RoleScope.h p2p/node/module/INodeModule.h p2p/role/RoleDefinitions.cpp p2p/role/RoleDefinitions.h p2p/node/module/ModuleState.h p2p/node/module/INodeModule.cpp p2p/node/configuration/DataFileNotFoundException.cpp p2p/node/configuration/DataFileNotFoundException.h p2p/node/module/logic/ModuleSource.cpp p2p/node/module/logic/ModuleSource.h p2p/node/module/logic/events/ModuleEvent.cpp p2p/node/module/logic/events/ModuleEvent.h p2p/dummy.cpp)


add_library(p2p_core ${SOURCE_FILES})
target_include_directories(p2p_core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_link_libraries(p2p_core logic ${Poco_LIBRARIES} ${LIBS})
install(TARGETS p2p_core COMPONENT p2p DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
install(DIRECTORY p2p DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
add_subdirectory(test)
add_subdirectory(p2p/modules)


add_library(p2p p2p/dummy.cpp)
target_link_libraries(p2p PUBLIC p2p_core logic module_auth module_basic module_filesystem module_command PRIVATE ${Poco_LIBRARIES} ${LIBS} spdlog::spdlog)
install(TARGETS p2p COMPONENT p2p DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)