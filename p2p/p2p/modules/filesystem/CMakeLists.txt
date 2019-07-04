

set(SOURCE_FILES FilesystemModule.cpp FilesystemModule.h identification/ResourceIdentificator.cpp identification/ResourceIdentificator.h identification/SimplePathRI.cpp identification/SimplePathRI.h network/packet/BeginTransfer.cpp network/packet/BeginTransfer.h network/packet/FinishTransfer.cpp network/packet/FinishTransfer.h identification/TransferTypes.h network/logic/events/TransferEvent.cpp network/logic/events/TransferEvent.h network/logic/sources/TransferSource.cpp network/logic/sources/TransferSource.h transfer/TransferManager.cpp transfer/TransferManager.h network/packet/DataTransfer.cpp network/packet/DataTransfer.h network/packet/PropertiesTransfer.cpp network/packet/PropertiesTransfer.h data/FileData.cpp data/FileData.h network/packet/TransferQuery.cpp network/packet/TransferQuery.h network/processor/NetworkProcessors.cpp network/processor/NetworkProcessors.h transfer/TransferException.cpp transfer/TransferException.h)


set(P2P_MODULE_DEPENDENCIES module_basic module_network)
include(${TEMPLATE_PATH}/p2p_module.cmake)