

set(SOURCE_FILES CommandModule.cpp CommandModule.h network/packet/CommandPacket.cpp network/packet/CommandPacket.h ICommandsDirectory.h network/logic/events/CommandEvent.cpp network/logic/events/CommandEvent.h network/logic/actions/CommandActions.cpp network/logic/actions/CommandActions.h)


set(P2P_MODULE_DEPENDENCIES module_basic module_network)
include(${TEMPLATE_PATH}/p2p_module.cmake)