set(BSC_P2P_BUILD_INCLUDE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

add_subdirectory(p2p/core)
add_subdirectory(p2p/modules)
if(${BSC_ENABLE_TESTS})
add_subdirectory(test)
endif()


install(DIRECTORY p2p DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
add_library(p2p p2p/dummy.cpp)
target_link_libraries(p2p PUBLIC p2p_core logic module_auth module_basic module_filesystem module_command PRIVATE)
target_precompile_headers(p2p REUSE_FROM logic)
install(TARGETS p2p COMPONENT p2p DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)