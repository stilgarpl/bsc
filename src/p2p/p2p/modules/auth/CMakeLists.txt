

set(SOURCE_FILES network/packet/AuthHelloPacket.h AuthModule.cpp AuthModule.h logic/sources/AuthSource.cpp logic/sources/AuthSource.h logic/events/AuthHelloEvent.cpp logic/events/AuthHelloEvent.h logic/actions/AuthActions.cpp logic/actions/AuthActions.h)

set(P2P_MODULE_DEPENDENCIES module_basic)
include(${TEMPLATE_PATH}/p2p_module.cmake)

