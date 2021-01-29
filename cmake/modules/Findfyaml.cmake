include(FindPackageHandleStandardArgs)
include(CheckSymbolExists)

#check if fyaml wasn't found already
if (NOT fyaml_FOUND)

    #found header and library
    find_path(fyaml_INCLUDE_PATH "libfyaml.h")
    #@todo fix library name
    find_library(fyaml_LIBRARIES NAMES fyaml libfyaml libfyaml.so libfyaml-0.5.so)
    message("fyaml: ${fyaml_INCLUDE_PATH} -- ${fyaml_LIBRARIES} --")
    if (fyaml_INCLUDE_PATH AND fyaml_LIBRARIES)
        #check if found fyaml library has right symbol
        set(CMAKE_REQUIRED_LIBRARIES ${fyaml_LIBRARIES})
    endif ()


    find_package_handle_standard_args(fyaml
            DEFAULT_MSG
            fyaml_LIBRARIES fyaml_INCLUDE_PATH)

    #Modern Cmake - export fyaml::fyaml target
    if (fyaml_FOUND AND NOT TARGET fyaml::fyaml)
        add_library(fyaml::fyaml UNKNOWN IMPORTED)
        set_target_properties(fyaml::fyaml PROPERTIES
                IMPORTED_LOCATION "${fyaml_LIBRARIES}"
                INTERFACE_INCLUDE_DIRECTORIES "${fyaml_INCLUDE_PATH}"
                )
    endif ()
    mark_as_advanced(fyaml_LIBRARIES fyaml_INCLUDE_PATH)

endif (NOT fyaml_FOUND)