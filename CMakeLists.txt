cmake_minimum_required(VERSION 3.16)
project(bsc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(TEMPLATE_PATH "${CMAKE_SOURCE_DIR}/cmake/template/")

#include additional configuration
include(${CMAKE_SOURCE_DIR}/cmake/configuration/sanitizers.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/configuration/analyzers.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/configuration/speedup.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/configuration/packagemanagers.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/configuration/compilers.cmake)


#testing
enable_testing()
find_package(Catch2 CONFIG REQUIRED)
include(Catch)
option(BSC_ENABLE_TESTS "Enable unit tests" ON)


#set destinations for install targets
include(GNUInstallDirs)
set(BSC_DESTINATION_LIB "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
set(BSC_DESTINATION_INCLUDE "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
set(BSC_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")


add_subdirectory(tester)
add_subdirectory(io)
add_subdirectory(parser)
add_subdirectory(core)
add_subdirectory(logic)
add_subdirectory(p2p)
add_subdirectory(repo)
add_subdirectory(bsc)


install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${BSC_CMAKE_DIR}
        )

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${BSC_CMAKE_DIR}
        )
# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        COMPATIBILITY SameMinorVersion
)

# install the configuration file
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${BSC_CMAKE_DIR}
        )

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR TGZ;)
include(CPack)
