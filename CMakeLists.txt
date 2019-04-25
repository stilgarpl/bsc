cmake_minimum_required(VERSION 3.14)
project(basyco VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
message($ENV{PATH})
#conan setup
find_program(conan NAMES conan)
message("DEBUG: conan = ${conan}")
if (EXISTS ${conan})
    message("CONAN EXISTS")
    include(${CMAKE_SOURCE_DIR}/conan/conan.cmake)
    conan_check()
    conan_add_remote(NAME bincrafters INDEX 1
            URL https://api.bintray.com/conan/bincrafters/public-conan)
    conan_cmake_run(CONANFILE conan/conanfile.txt
            INSTALL_FOLDER ${CMAKE_BINARY_DIR}
            BASIC_SETUP
            BUILD missing)
    set(LIBS ${LIBS} ${CONAN_LIBS})
    set(INCLUDES ${CONAN_INCLUDE_DIRS})
else ()
    message("Conan not found")
endif ()
#end of conan setup


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-pthread -lstdc++fs -Wall -Wextra -Wno-unused-local-typedefs -Wno-unused-parameter -pedantic)
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lstdc++fs -Wall -Wno-unused-local-typedefs -pedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if (MSVC_VERSION GREATER_EQUAL "1900")
        include(CheckCXXCompilerFlag)
        add_compile_options("/DNOMINMAX")
    endif()
endif ()


add_subdirectory(p2p)
add_subdirectory(repo)
add_subdirectory(basyco)
