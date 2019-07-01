

set(SOURCE_FILES BasicModule.cpp BasicModule.h)

get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

add_library(${MODULE_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${MODULE_NAME} p2p_core)
install(TARGETS ${MODULE_NAME} COMPONENT p2p)
