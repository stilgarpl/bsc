//
// Created by stilgar on 31.07.17.
//


#include "Node.h"
#include <p2p/node/context/NodeContext.h>
#include <core/log/LoggerContext.h>
#include <core/factory/FactoryContext.h>
#include <core/io/InputOutputContext.h>
#include "p2p/node/module/NodeModule.h"


bsc::Node::~Node() {
    std::unique_lock<std::recursive_mutex> g(startMutex);
    //setting node context to active - if more than one node is created in a single thread, things may get mixed up.
    Node::setNodeContextActive();
    if (started) {
        ERROR("stopping in destructor is usually too late. stop and wait!")
        Node::stop();
        bsc::Node::waitToFinish();
    }


}


void bsc::Node::start() {
    std::unique_lock<std::recursive_mutex> g(startMutex);
    setNodeContextActive();
    initialize();
    logicManager.start();
    startModules();
    started = true;
    startedReady.notify_all();

}

void bsc::Node::stop() {
    LOGGER("node stop")
    std::unique_lock<std::recursive_mutex> g(startMutex);
    setNodeContextActive();
    started = false;
    shutdownModules();
    //@todo wait for shutdown to complete, sleep will do for now
    std::this_thread::sleep_for(10ms);
    logicManager.stop();
    stopModules();
}


bsc::Node::Node() {
    LOGGER("default node constructor")
    nodeContext->set<NodeContext, Node&, NodeInfo&>(*this, this->thisNodeInfo);
    nodeContext->set<bsc::LoggerContext>([&] { return thisNodeInfo.getNodeId(); });
    nodeContext->setDirect<bsc::InputOutputContext>(std::make_shared<bsc::StandardInputOutputContext>());

    //this creates common context for role definitions across the entire node.
    nodeContext->set<RoleDefinitionsContext>();
    nodeContext->setDebugId("node contxt");

    nodeContext->set<bsc::FactoryContext>();

    logicManager.setContexts(nodeContext);

    Node::setNodeContextActive();
    LOGGER("node constructor finished")
}


void bsc::Node::startModules() {
    forEachModule<void>(&bsc::NodeModule::start);
}

void bsc::Node::initialize() {
    setNodeContextActive();

//    //@todo debug values
    configurationManager.setRootPath(nodeConfiguration.rootPath / this->getNodeInfo().getNodeId());

    //this slightly changed the order of execution - instead of being initialized and setupLogic module by module,
    //now all modules are initialized and then all modules are setupLogiced
    //hope it doesn't break anything
    forEachModule(&bsc::INodeModule::initializeConfiguration);
    forEachModule(&bsc::INodeModule::doInitialize);
    forEachModule(&bsc::INodeModule::doSetupLogic);
    forEachModule(&bsc::INodeModule::doPrepareSubmodules);
    forEachModule(&bsc::INodeModule::doReady);

}

void bsc::Node::stopModules() {
    forEachModule(&bsc::NodeModule::stop);

}

void bsc::Node::joinModules() {
    forEachModule(&bsc::NodeModule::join);

}

void bsc::Node::waitToFinish() {
    std::unique_lock<std::recursive_mutex> g(startMutex);
    logicManager.join();
    joinModules();

}

void bsc::Node::shutdownModules() {
    forEachModule(&bsc::INodeModule::doShutdown);


}

void bsc::Node::saveConfiguration() {
    forEachModule(&bsc::INodeModule::doSaveConfiguration);

}

void bsc::Node::waitUntilStarted() {
    std::unique_lock<std::recursive_mutex> g(startMutex);
    startedReady.wait(g, [this] { return started; });
}

bsc::Node::Configuration::Configuration() {

    //@todo constant or sth for project name
    rootPath = fs::path(std::getenv("HOME")) / ".basyco";
//    LOGGER("root path is " + rootPath.string());
}
