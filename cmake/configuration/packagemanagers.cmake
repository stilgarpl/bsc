
option(BSC_USE_PACKAGE_MANAGER "Use package managers to build this project" OFF)
if (BSC_USE_PACKAGE_MANAGER)
    #conan setup
    find_program(conan NAMES conan)
    message("DEBUG: conan = ${conan}")
    if (EXISTS ${conan})
        message("CONAN EXISTS")
        set(CONAN_SYSTEM_INCLUDES ON)
        include(${CMAKE_SOURCE_DIR}/conan/conan.cmake)
#        conan_add_remote(NAME bincrafters INDEX 1
#                URL https://api.bintray.com/conan/bincrafters/public-conan)
#        conan_add_remote(NAME degoodmanwilson INDEX 2
#                URL https://api.bintray.com/conan/degoodmanwilson/opensource)
        conan_cmake_run(CONANFILE conan/conanfile.txt
                INSTALL_FOLDER ${CMAKE_BINARY_DIR}
                BASIC_SETUP
                BUILD_TYPE "Debug"
                BUILD missing)
        include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
    else ()
        message("Conan not found")
    endif ()

    #end of conan setup
endif ()