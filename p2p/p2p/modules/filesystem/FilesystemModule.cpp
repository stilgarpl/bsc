//
// Created by stilgar on 07.11.17.
//

#include <p2p/modules/filesystem/network/packet/TransferQuery.h>
#include "FilesystemModule.h"

#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/filesystem/network/processor/FilesystemNetworkProcessors.h>
#include <p2p/modules/filesystem/network/packet/BeginTransfer.h>
#include <p2p/modules/filesystem/network/packet/FinishTransfer.h>
#include <p2p/modules/filesystem/network/packet/DataTransfer.h>
#include <p2p/modules/filesystem/network/packet/PropertiesTransfer.h>
namespace bsc {
    FilesystemModule::FilesystemModule(INode& node) : NodeModuleDependent<FilesystemModule, NetworkModule>(node,
                                                                                                           "filesystem") {
//    setRequired<BasicModule, NetworkModule>();
    }

    void FilesystemModule::setupActions(ILogicModule::SetupActionHelper& actionHelper) {

        using namespace std::placeholders;

//    actionHelper.setAction<TransferEvent>("beginTransfer",
//                                          std::bind(&TransferManager::beginTransfer, &this->transferManager, _1));
//    actionHelper.setAction<TransferEvent>("finishTransfer",
//                                          std::bind(&TransferManager::finishTransfer, &this->transferManager, _1));
//    actionHelper.setAction<TransferEvent>("sendData",
//                                          std::bind(&TransferManager::sendData, &this->transferManager, _1));
//    actionHelper.setAction<TransferEvent>("sendProps",
//                                          std::bind(&TransferManager::transferProperties, &this->transferManager, _1));

    }

    bool FilesystemModule::assignActions(ILogicModule::AssignActionHelper& actionHelper) {


        {
//        //@todo using enum TransferEvent::IdType;
//        when(event<TransferEvent>(TransferEvent::IdType::REQUESTED)).fireAction("beginTransfer");
//        when(event<TransferEvent>(TransferEvent::IdType::SENDING)).fireAction("sendData");
//        when(event<TransferEvent>(TransferEvent::IdType::GET_PROPERTIES)).fireAction("sendProps");
//        when(event<TransferEvent>(TransferEvent::IdType::FINISHING)).fireAction("finishTransfer");


        }
        when(state<TransferManager::LocalTransferDescriptor>(
                TransferManager::TransferState::STARTED).entered()).fireStateChangeReaction(
                [](TransferManager::LocalTransferDescriptor& descriptor) { LOGGER("transfer started!!!!...!!"); });
//        when(event<LogicStateEvent<TransferManager::LocalTransferDescriptor,TransferStatus >>()).fireNewAction([](auto event){LOGGER("transfer state evennnnnt")});
        return true;
    }

    bool FilesystemModule::setupSources(ILogicModule::SetupSourceHelper& sourceHelper) {
//    sourceHelper.requireSource<TransferSource>();
        return true;
    }

    const std::filesystem::path& FilesystemModule::getCurrentPath() const {
        return currentPath;
    }

    void FilesystemModule::prepareSubmodules() {
        auto& networkSub = getSubModule<NetworkModule>();
        networkSub.registerPacketProcessor<TransferQuery>(FilesystemNetworkProcessors::queryProcessor);
        networkSub.registerPacketProcessor<BeginTransfer>([this](BeginTransfer::Request::Ptr request) {
            BeginTransfer::Response::Ptr response = BeginTransfer::Response::getNew();
            auto transferId = transferManager.beginTransfer(request->getResourceId());
            response->setTransferId(transferId);
            return response;
        });

        networkSub.registerPacketProcessor<FinishTransfer>([this](FinishTransfer::Request::Ptr request) {
            FinishTransfer::Response::Ptr response = FinishTransfer::Response::getNew();
            transferManager.finishTransfer(request->getTransferId());
            //@todo maybe return success or failure?
            return response;
        });

        networkSub.registerPacketProcessor<DataTransfer>([this](DataTransfer::Request::Ptr request) {
            DataTransfer::Response::Ptr response = DataTransfer::Response::getNew();
            auto[transferData, begin, end] = transferManager.sendData(request->getTransferId(), request->getBegin(),
                                                                      request->getEnd());
            //@todo avoid vector copy here:
            response->setData(transferData);
            response->setBegin(begin);
            response->setEnd(end);
            return response;
        });

        networkSub.registerPacketProcessor<PropertiesTransfer>([this](PropertiesTransfer::Request::Ptr request) {
            PropertiesTransfer::Response::Ptr response = PropertiesTransfer::Response::getNew();
            auto size = transferManager.transferProperties(request->getTransferId());
            //@todo other properties
            response->setSize(size);
            return response;
        });
    }

}