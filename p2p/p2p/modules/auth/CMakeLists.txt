

set(SOURCE_FILES network/packet/AuthHelloPacket.cpp network/packet/AuthHelloPacket.h AuthModule.cpp AuthModule.h logic/sources/AuthSource.cpp logic/sources/AuthSource.h logic/events/AuthHelloEvent.cpp logic/events/AuthHelloEvent.h logic/actions/AuthActions.cpp logic/actions/AuthActions.h)

get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

add_library(${MODULE_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${MODULE_NAME} p2p_core basic)
install(TARGETS ${MODULE_NAME} COMPONENT p2p)
