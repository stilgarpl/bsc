//
// Created by stilgar on 31.07.17.
//


#include "Node.h"
#include <p2p/node/context/NodeContext.h>
#include <core/log/LoggerContext.h>
#include <core/factory/FactoryContext.h>
#include <core/io/InputOutputContext.h>
#include "p2p/node/module/NodeModule.h"


Node::~Node() {
    std::unique_lock <std::recursive_mutex> g(startMutex);
    //setting node context to active - if more than one node is created in a single thread, things may get mixed up.
    Node::setNodeContextActive();
    if (started) {
        ERROR("stopping in destructor is usually too late. stop and wait!")
        Node::stop();
        Node::waitToFinish();
    }


}



void Node::start() {
    std::unique_lock <std::recursive_mutex> g(startMutex);
    setNodeContextActive();
    initialize();
    logicManager.start();
    startModules();
    started = true;
    startedReady.notify_all();

}

void Node::stop() {
    LOGGER("node stop")
    std::unique_lock <std::recursive_mutex> g(startMutex);
    setNodeContextActive();
    started = false;
    shutdownModules();
    //@todo wait for shutdown to complete, sleep will do for now
    std::this_thread::sleep_for(10ms);
    logicManager.stop();
    stopModules();
}


Node::Node() {
    LOGGER("default node constructor")
    nodeContext->set<NodeContext, Node &, NodeInfo &>(*this, this->thisNodeInfo);
    nodeContext->set<LoggerContext>([&] { return thisNodeInfo.getNodeId(); });
    nodeContext->setContext<InputOutputContext, StandardInputOutputContext>();

    //this creates common context for role definitions across the entire node.
    nodeContext->set<RoleDefinitionsContext>();
    nodeContext->setDebugId("node contxt");

    nodeContext->set<FactoryContext>();

    logicManager.setContexts(nodeContext);

    Node::setNodeContextActive();
    LOGGER("node constructor finished")
}


void Node::startModules() {
    forEachModule<void>(&NodeModule::start);
}

void Node::initialize() {
    setNodeContextActive();

//    //@todo debug values
    configurationManager.setRootPath(nodeConfiguration.rootPath / this->getNodeInfo().getNodeId());

    //this slightly changed the order of execution - instead of being initialized and setupLogic module by module,
    //now all modules are initialized and then all modules are setupLogiced
    //hope it doesn't break anything
    forEachModule(&INodeModule::initializeConfiguration);
    forEachModule(&INodeModule::doInitialize);
    forEachModule(&INodeModule::doSetupLogic);
    forEachModule(&INodeModule::doPrepareSubmodules);
    forEachModule(&INodeModule::doReady);

}

void Node::stopModules() {
    forEachModule(&NodeModule::stop);

}

void Node::joinModules() {
    forEachModule(&NodeModule::join);

}

void Node::waitToFinish() {
    std::unique_lock <std::recursive_mutex> g(startMutex);
    logicManager.join();
    joinModules();

}

void Node::shutdownModules() {
    forEachModule(&INodeModule::doShutdown);


}

void Node::saveConfiguration() {
    forEachModule(&INodeModule::doSaveConfiguration);

}

void Node::waitUntilStarted() {
    std::unique_lock <std::recursive_mutex> g(startMutex);
    startedReady.wait(g, [this] { return started; });
}

Node::Configuration::Configuration() {

    //@todo constant or sth for project name
    rootPath = fs::path(std::getenv("HOME")) / ".basyco";
//    LOGGER("root path is " + rootPath.string());
}
