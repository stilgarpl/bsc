

set(SOURCE_FILES FilesystemModule.cpp FilesystemModule.h identification/ResourceIdentificator.cpp identification/ResourceIdentificator.h identification/SimplePathRI.cpp identification/SimplePathRI.h network/packet/BeginTransfer.cpp network/packet/BeginTransfer.h network/packet/FinishTransfer.cpp network/packet/FinishTransfer.h identification/TransferTypes.h network/logic/events/TransferEvent.cpp network/logic/events/TransferEvent.h network/logic/sources/TransferSource.cpp network/logic/sources/TransferSource.h transfer/TransferManager.cpp transfer/TransferManager.h network/packet/DataTransfer.cpp network/packet/DataTransfer.h network/packet/PropertiesTransfer.cpp network/packet/PropertiesTransfer.h data/FileData.cpp data/FileData.h network/packet/TransferQuery.cpp network/packet/TransferQuery.h network/processor/NetworkProcessors.cpp network/processor/NetworkProcessors.h transfer/TransferException.cpp transfer/TransferException.h)


get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

add_library(${MODULE_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${MODULE_NAME} p2p_core network)
install(TARGETS ${MODULE_NAME} COMPONENT p2p)
