

set(SOURCE_FILES CommandModule.cpp CommandModule.h network/packet/CommandPacket.cpp network/packet/CommandPacket.h ICommandsDirectory.h StandardCommandsDirectory.h network/logic/sources/CommandSource.cpp network/logic/sources/CommandSource.h network/logic/events/CommandEvent.cpp network/logic/events/CommandEvent.h network/logic/actions/CommandActions.cpp network/logic/actions/CommandActions.h)


get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

add_library(${MODULE_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${MODULE_NAME} p2p_core basic network)
install(TARGETS ${MODULE_NAME} COMPONENT p2p)
