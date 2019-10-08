cmake_minimum_required(VERSION 3.14)
project(basyco VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(TEMPLATE_PATH "${CMAKE_SOURCE_DIR}/cmake/template/")

#include additional configuration
include(${CMAKE_SOURCE_DIR}/cmake/configuration/sanitizers.cmake)
enable_testing()


include(GNUInstallDirs)

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
            CMAKE_TARGETS
            BUILD_TYPE "Debug"
            BUILD missing)
    set(LIBS ${LIBS} ${CONAN_LIBS})
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
        add_compile_options("/DNOMINMAX")
    endif ()
endif ()


# Generate clang compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(core)
add_subdirectory(logic)
add_subdirectory(p2p)
add_subdirectory(repo)
add_subdirectory(basyco)


set(CPACK_GENERATOR TGZ;DEB;RPM;)
include(CPack)
