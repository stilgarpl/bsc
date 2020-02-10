find_package(Poco CONFIG REQUIRED COMPONENTS Net Util Foundation REQUIRED)
find_package(Cereal CONFIG REQUIRED)

set(SOURCE_FILES node/NodeInfo.cpp node/NodeInfo.h node/Node.cpp node/Node.h node/context/NodeContext.cpp node/context/NodeContext.h node/configuration/ConfigurationManager.cpp node/configuration/ConfigurationManager.h node/configuration/IConfig.h node/module/logic/ILogicModule.cpp node/module/logic/ILogicModule.h node/module/NodeModule.cpp node/module/NodeModule.h node/INode.h dependency/DependencyManaged.h dependency/DependencyManager.h dependency/Dependency.h dependency/IDependency.h dependency/IDependencyManaged.h role/Role.cpp role/Role.h role/Roles.cpp role/Roles.h role/RoleInvalidException.cpp role/RoleInvalidException.h role/RoleScope.cpp role/RoleScope.h node/module/INodeModule.h role/RoleDefinitions.cpp role/RoleDefinitions.h node/module/ModuleState.h node/module/INodeModule.cpp node/configuration/DataFileNotFoundException.cpp node/configuration/DataFileNotFoundException.h node/module/logic/ModuleSource.cpp node/module/logic/ModuleSource.h node/module/logic/events/ModuleEvent.cpp node/module/logic/events/ModuleEvent.h)


add_library(p2p_core ${SOURCE_FILES})
target_include_directories(p2p_core PUBLIC ${BSC_P2P_BUILD_INCLUDE} $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_link_libraries(p2p_core PUBLIC logic Poco::Net ${LIBS})
install(TARGETS p2p_core COMPONENT p2p DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)