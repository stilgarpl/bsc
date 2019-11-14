

get_filename_component(MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(MODULE_NAME module_${MODULE_NAME})
add_library(${MODULE_NAME} ${SOURCE_FILES})
target_link_libraries(${MODULE_NAME} p2p_core ${P2P_MODULE_DEPENDENCIES})
install(TARGETS ${MODULE_NAME} COMPONENT p2p)
