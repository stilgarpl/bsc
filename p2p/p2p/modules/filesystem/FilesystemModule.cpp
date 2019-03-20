//
// Created by stilgar on 07.11.17.
//

#include <p2p/modules/filesystem/network/logic/sources/TransferSource.h>
#include <p2p/modules/filesystem/network/packet/TransferQuery.h>
#include "FilesystemModule.h"
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/filesystem/network/processor/NetworkProcessors.h>

FilesystemModule::FilesystemModule(INode &node) : NodeModuleDependent<FilesystemModule, NodeNetworkModule>(node) {
//    setRequired<BasicModule, NodeNetworkModule>();
}

void FilesystemModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {

    using namespace std::placeholders;

    actionHelper.setAction<TransferEvent>("beginTransfer",
                                          std::bind(&TransferManager::beginTransfer, &this->transferManager, _1));
    actionHelper.setAction<TransferEvent>("finishTransfer",
                                          std::bind(&TransferManager::finishTransfer, &this->transferManager, _1));
    actionHelper.setAction<TransferEvent>("sendData",
                                          std::bind(&TransferManager::sendData, &this->transferManager, _1));
    actionHelper.setAction<TransferEvent>("sendProps",
                                          std::bind(&TransferManager::transferProperties, &this->transferManager, _1));

}

bool FilesystemModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    if (actionHelper.assignAction<TransferEvent>(TransferEvent::IdType::REQUESTED, "beginTransfer") &&
        actionHelper.assignAction<TransferEvent>(TransferEvent::IdType::SENDING, "sendData") &&
        actionHelper.assignAction<TransferEvent>(TransferEvent::IdType::GET_PROPERTIES, "sendProps") &&
        actionHelper.assignAction<TransferEvent>(TransferEvent::IdType::FINISHING, "finishTransfer") //&&
            ) {
        std::clog << "Debug: File assignment!" << std::endl;

        when(state<TransferManager::LocalTransferDescriptor>(
                TransferManager::TransferState::STARTED).entered()).fireStateChangeReaction(
                [](TransferManager::LocalTransferDescriptor &descriptor) { LOGGER("transfer started!!!!...!!"); });
//        when(event<LogicStateEvent<TransferManager::LocalTransferDescriptor,TransferStatus >>()).fireNewAction([](auto event){LOGGER("transfer state evennnnnt")});
        return true;
    }
    return false;
}

bool FilesystemModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<TransferSource>();
    return true;
}

const std::filesystem::path &FilesystemModule::getCurrentPath() const {
    return currentPath;
}

void FilesystemModule::prepareSubmodules() {
    auto &networkSub = getSubModule<NodeNetworkModule>();
    networkSub.registerPacketProcessor<TransferQuery>(NetworkProcessors::queryProcessor);
}

