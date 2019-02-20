cmake_minimum_required(VERSION 3.7)
project(basyco)
find_program(conan conan)
if(EXISTS ${conan})
message("CONAN EXISTS")
include(${CMAKE_SOURCE_DIR}/conan/conan.cmake)

conan_cmake_run(CONANFILE conan/conanfile.txt
        INSTALL_FOLDER ${CMAKE_BINARY_DIR}
        BASIC_SETUP
        BUILD missing)
set(LIBS ${LIBS} ${CONAN_LIBS})
endif()

#if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#    conan_basic_setup()
#else()
#    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
#endif()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lstdc++fs -Wall -Wno-unused-local-typedefs")



add_subdirectory(p2p)
add_subdirectory(repo)
add_subdirectory(basyco)
