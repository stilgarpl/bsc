cmake_minimum_required(VERSION 3.14)
project(bsc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(TEMPLATE_PATH "${CMAKE_SOURCE_DIR}/cmake/template/")

#include additional configuration
include(${CMAKE_SOURCE_DIR}/cmake/configuration/sanitizers.cmake)
enable_testing()




find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}") # CMake 3.9+
endif ()


message($ENV{PATH})
#conan setup
find_program(conan NAMES conan)
message("DEBUG: conan = ${conan}")
if (EXISTS ${conan})
    message("CONAN EXISTS")
    set(CONAN_SYSTEM_INCLUDES ON)
    include(${CMAKE_SOURCE_DIR}/conan/conan.cmake)
    conan_check()
    conan_add_remote(NAME bincrafters INDEX 1
            URL https://api.bintray.com/conan/bincrafters/public-conan)
    conan_cmake_run(CONANFILE conan/conanfile.txt
            INSTALL_FOLDER ${CMAKE_BINARY_DIR}
            BASIC_SETUP
            BUILD_TYPE "Debug"
            BUILD missing)
    set(INCLUDES ${CONAN_INCLUDE_DIRS})

else ()
    message("Conan not found")
endif ()
#end of conan setup

find_package(Catch2 REQUIRED)
include(Catch)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wno-unused-local-typedefs -Wno-unused-parameter -pedantic)
    if (LINUX)
        add_compile_options(-pthread)
        add_link_options(-lpthread)
    endif ()

    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lstdc++fs -Wall -Wno-unused-local-typedefs -pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if (MSVC_VERSION GREATER_EQUAL "1900")
        include(CheckCXXCompilerFlag)
        add_compile_definitions(NOGDI)
        add_compile_definitions(NOMINMAX)
        add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)
        add_compile_definitions(_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
        add_compile_options(/bigobj)
        include_directories(SYSTEM C:/Program\ Files \(x86\)/Windows\ Kits/10/Include/10.0.18362.0/shared)
    endif ()
endif ()

#set destinations for install targets
include(GNUInstallDirs)
set(BSC_DESTINATION_LIB "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
set(BSC_DESTINATION_INCLUDE "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
set(BSC_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")


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
