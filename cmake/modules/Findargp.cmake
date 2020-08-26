# This file is part of CMake-argp.
#
# CMake-argp is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along
# with this program. If not, see
#
#  http://www.gnu.org/licenses/
#
#
# Copyright (c)
#   2016-2017 Alexander Haase <ahaase@alexhaase.de>
#

include(FindPackageHandleStandardArgs)
include(CheckFunctionExists)


# First search the argp header file. If it is not found, any further steps will
# fail.
find_path(argp_INCLUDE_PATH "argp.h")
if (argp_INCLUDE_PATH)
    set(CMAKE_REQUIRED_INCLUDES "${argp_INCLUDE_PATH}")

    # Find the required argp library. argp may be shipped together with libc (as
    # glibc does), or as independent library (e.g. for Windows, mac OS, ...). If
    # the library was found before, the cached result will be used.
    if (NOT argp_LIBRARIES)
        # First check if argp is shipped together with libc. The required
        # argp_parse function should be available after linking to libc,
        # otherwise libc doesn't ship it.
        check_function_exists("argp_parse" argp_IN_LIBC)
        if (argp_IN_LIBC)
            # argp is in standard lib c
        else ()
            # argp is not shipped with libc. Try to find the argp library and check
            # if it has the required argp_parse function.
            find_library(argp_LIBRARIES NAMES "argp")
            if (argp_LIBRARIES)
                set(CMAKE_REQUIRED_LIBRARIES "${argp_LIBRARIES}")
                check_function_exists("argp_parse" argp_EXTERNAL)
                if (NOT argp_EXTERNAL)
                    message(FATAL_ERROR "Your system ships an argp library in "
                            "${argp_LIBRARIES}, but it does not have a symbol "
                            "named argp_parse.")
                endif ()
            endif ()
        endif ()
    endif ()
endif ()


# Check for all required variables.
find_package_handle_standard_args(argp
        DEFAULT_MSG
        argp_INCLUDE_PATH)

if (argp_FOUND AND NOT TARGET argp::argp)

    if (DEFINED argp_LIBRARIES)
        add_library(argp::argp UNKNOWN IMPORTED)
        set_target_properties(argp::argp PROPERTIES
                IMPORTED_LOCATION "${argp_LIBRARIES}"
                INTERFACE_INCLUDE_DIRECTORIES "${argp_INCLUDE_PATH}"
                )
    else ()
        add_library(argp::argp INTERFACE IMPORTED)
        set_target_properties(argp::argp PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${argp_INCLUDE_PATH}"
                )
    endif ()
endif ()
mark_as_advanced(argp_LIBRARIES argp_INCLUDE_PATH)
