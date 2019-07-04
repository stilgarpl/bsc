//
// Created by stilgar on 31.07.17.
//


#include "Node.h"
#include <p2p/node/context/NodeContext.h>
#include <logic/context/LogicContext.h>
#include <p2p/node/configuration/ConfigurationManager.h>
#include <core/log/LoggerContext.h>
#include "p2p/node/module/NodeModule.h"


Node::~Node() {
    //setting node context to active - if more than one node is created in a single thread, things may get mixed up.
    setNodeContextActive();
    if (started) {
        ERROR("stopping in destructor is usually too late. stop and wait!")
        stop();
    }


}



void Node::start() {
    std::unique_lock<std::mutex> g(startMutex);
    initialize();
    logicManager.start();
    startModules();
    started = true;
    startedReady.notify_all();

}

void Node::stop() {
    LOGGER("node stop")
    std::unique_lock<std::mutex> g(startMutex);
    started = false;
    shutdownModules();
    //@todo wait for shutdown to complete, sleep will do for now
    std::this_thread::sleep_for(10ms);
    logicManager.stop();
    stopModules();
}


Node::Node() {

    nodeContext->set<NodeContext, Node &, NodeInfo &>(*this, this->thisNodeInfo);
    //@todo setting of LogicContext should probably be done inside LogicManager
    nodeContext->set<LogicContext, LogicManager &>(logicManager);
    nodeContext->set<LoggerContext>([&] { return thisNodeInfo.getNodeId(); });

    //this creates common context for role definitions across the entire node.
    nodeContext->set<RoleDefinitionsContext>();
    nodeContext->setDebug_id("node contxt");

    logicManager.setContexts(nodeContext);

    setNodeContextActive();

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
    logicManager.join();
    joinModules();
//    //@todo replace this with waiting for modules
//    std::this_thread::sleep_for(1s);

}

void Node::shutdownModules() {
    forEachModule(&INodeModule::doShutdown);


}

void Node::saveConfiguration() {
    forEachModule(&INodeModule::doSaveConfiguration);

}

void Node::waitUntilStarted() {
    std::unique_lock<std::mutex> g(startMutex);
    startedReady.wait(g, [this] { return started; });
}

Node::Configuration::Configuration() {

    //@todo constant or sth for project name
    rootPath = fs::path(std::getenv("HOME")) / ".basyco";
//    LOGGER("root path is " + rootPath.string());
}
