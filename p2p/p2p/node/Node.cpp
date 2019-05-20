//
// Created by stilgar on 31.07.17.
//


#include "Node.h"
#include <p2p/node/context/NodeContext.h>
#include <p2p/node/context/LogicContext.h>
#include <p2p/modules/configuration/ConfigurationManager.h>
#include "NodeModule.h"


Node::~Node() {


    stop();


}



void Node::start() {

    initialize();

    logicManager.start();

    startModules();


}

void Node::stop() {

    shutdownModules();
    //@todo wait for shutdown to complete, sleep will do for now
    std::this_thread::sleep_for(1s);
    logicManager.stop();
    stopModules();

}


Node::Node() {

    nodeContext->set<NodeContext, Node &, NodeInfo &>(*this, this->thisNodeInfo);
    nodeContext->set<LogicContext, LogicManager &>(logicManager);

    //this creates common context for role definitions across the entire node.
    nodeContext->set<RoleDefinitionsContext>();
    nodeContext->setDebug_id("node contxt");

    logicManager.setContexts(nodeContext);



}

Node::Node(int port) : Node() {
//    std::shared_ptr<Node::Config> config = std::make_shared<Node::Config>();
//    config->setPort(port);
//    setConfiguration(config);

    //@todo debug remove
//    ConfigurationManager manager;
//    manager.save("Node", config);

//    ConfigurationModule



//    thisNodeInfo.addKnownAddress("127.0.0.1:" + std::to_string(getConfiguration()->getPort()));


}


void Node::startModules() {
    forEachModule<void>(&NodeModule::start);
}

void Node::initialize() {
    setNodeContextActive();

//    //@todo debug values
    configurationManager.setRootPath(fs::path("/tmp/basyco") / this->getNodeInfo().getNodeId());

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

Node::Configuration::Configuration() {

    rootPath = std::getenv("HOME");
    LOGGER("root path is " + rootPath.string());
}
