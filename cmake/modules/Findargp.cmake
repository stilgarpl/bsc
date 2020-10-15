include(FindPackageHandleStandardArgs)
include(CheckSymbolExists)

#check if argp wasn't found already
if (NOT argp_FOUND)

    #found header and library
    find_path(argp_INCLUDE_PATH "argp.h")
    #here cmake is clever, because looking for "argp" will return libc if argp is included in it (GNU libc)
    find_library(argp_LIBRARIES NAMES argp)
    if (argp_INCLUDE_PATH AND argp_LIBRARIES)
        #check if found argp library has right symbol
        set(CMAKE_REQUIRED_LIBRARIES ${argp_LIBRARIES})
        check_symbol_exists("argp_parse" "argp.h" argp_parse_WORKS)
        if (NOT argp_parse_WORKS)
            message(FATAL_ERROR "Argp found at ${argp_LIBRARIES} but argp_parse symbol does not exist")
        endif ()

    endif ()


    find_package_handle_standard_args(argp
            DEFAULT_MSG
            argp_LIBRARIES argp_INCLUDE_PATH)

    #Modern Cmake - export argp::argp target
    if (argp_FOUND AND NOT TARGET argp::argp)
        add_library(argp::argp UNKNOWN IMPORTED)
        set_target_properties(argp::argp PROPERTIES
                IMPORTED_LOCATION "${argp_LIBRARIES}"
                INTERFACE_INCLUDE_DIRECTORIES "${argp_INCLUDE_PATH}"
                )
    endif ()
    mark_as_advanced(argp_LIBRARIES argp_INCLUDE_PATH)

endif (NOT argp_FOUND)