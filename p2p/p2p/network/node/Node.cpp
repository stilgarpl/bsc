//
// Created by stilgar on 31.07.17.
//


#include "Node.h"
#include "p2p/network/node/protocol/packet/NodeInfoRequest.h"
#include "p2p/network/node/protocol/packet/NodeInfoResponse.h"
#include <Poco/Net/NetException.h>
#include <p2p/network/protocol/connection/ServerConnection.h>
#include <p2p/network/protocol/connection/ClientConnection.h>
#include <p2p/network/protocol/context/NodeContext.h>
#include <p2p/network/protocol/context/LogicContext.h>
#include <p2p/configuration/ConfigurationManager.h>




Node::~Node() {

    stop();


}



void Node::start() {

    initialize();

    logicManager.start();

    startModules();


}

void Node::stop() {

    logicManager.stop();
    stopModules();

}


void Node::work() {

//    for (auto &i : activeClientConnections) {
//        //@todo implement
//        //test if connection is still alive
//        //process all messages from receive queue
//    }

}

const std::shared_ptr<Node::Config> &Node::getConfiguration() const {
    return configuration;
}

void Node::setConfiguration(const std::shared_ptr<Node::Config> &configuration) {
    Node::configuration = configuration;
}

Node::Node() {

    nodeContext.set<NodeContext, Node &, NodeInfo &>(*this, this->thisNodeInfo);
    nodeContext.set<LogicContext, LogicManager &>(logicManager);

    logicManager.setContexts(nodeContext);



}

Node::Node(int port) : Node() {
    std::shared_ptr<Node::Config> config = std::make_shared<Node::Config>();
    config->setPort(port);
    setConfiguration(config);

    ///@todo debug remove
    ConfigurationManager manager;
    manager.save("Node", config);

    thisNodeInfo.addKnownAddress("127.0.0.1:" + std::to_string(getConfiguration()->getPort()));


}

std::shared_ptr<NetworkInfo> &Node::getNetworkInfo() {
    return networkInfo;
}





void Node::startModules() {
    forEachModule<void>(&NodeModule::start);
}

void Node::initialize() {
    //initialize node modules
//    std::list<INodeModulePtr> modulesList;
//    modules.forEach(
//            [&](INodeModulePtr ptr) {
//                if (ptr != nullptr) {
//                    //ptr->initialize();
//                    //ptr->setupLogic(logicManager);
//                    modulesList.push_back(ptr);
//                };
//            });
//    auto sortedList = DependencyManager::dependencySort(modulesList);
//
//    for (auto &&item : sortedList) {
//        item->initialize();
//        item->setupLogic(logicManager);
//    }

    //this slightly changed the order of execution - instead of being initialized and setupLogic module by module,
    //now all modules are initialized and then all modules are setupLogiced
    //hope it doesn't break anything
    forEachModule(&INodeModule::initialize);
    forEachModule<bool, ILogicModule, LogicManager &>(&ILogicModule::setupLogic, logicManager);

}

void Node::stopModules() {
    forEachModule(&NodeModule::stop);

}

void Node::joinModules() {
    forEachModule(&NodeModule::join);

}

INodeModulePtr Node::getModuleByDependencyId(DependencyManager::TypeIdType id) {
    INodeModulePtr result = nullptr;
    modules.forEach([&](auto i) {
        if (i->getDependencyId() == id) {
            result = i;
            return;
        }
    });
    return result;
}

