cmake_minimum_required(VERSION 3.7)
project(basyco VERSION 0.1 LANGUAGES CXX)

#conan setup
find_program(conan conan)
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
endif ()
#end of conan setup

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-pthread -lstdc++fs -Wall -Wno-unused-local-typedefs -pedantic)
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
