//
// Created by Krzysztof Tulidowicz on 31.07.17.
//

#include "Node.h"
#include "p2p/core/node/module/NodeModule.h"
#include <core/factory/FactoryContext.h>
#include <core/io/InputOutputContext.h>
#include <core/log/LoggerContext.h>
#include <p2p/core/node/context/NodeContext.h>

#include <utility>

bsc::Node::~Node() {
    std::unique_lock g(startMutex);
    // setting node context to active - if more than one node is created in a single thread, things may get mixed up.
    Node::setNodeContextActive();
    if (started) {
        ERROR("stopping in destructor is usually too late. stop and wait!")
        Node::stop();
        bsc::Node::waitToFinish();
    }
}
namespace bsc {
    bool Node::isRunning() {
        std::unique_lock g(startMutex);
        return started;
    }
}// namespace bsc

void bsc::Node::start() {
    std::unique_lock g(startMutex);
    setNodeContextActive();
    initialize();
    logicManager.start();
    startModules();//@todo this should wait for at least onStart() phase to complete for all modules. IMPORTANT!!!
    started = true;
    startedReady.notify_all();
}

void bsc::Node::stop() {
    LOGGER("node stop")
    std::unique_lock g(startMutex);
    setNodeContextActive();
    started = false;
    shutdownModules();//@todo as above, this should wait for completion of onStop() phase on Runnable IMPORTANT!!!
    //@todo wait for shutdown to complete, sleep will do for now
    std::this_thread::sleep_for(10ms);
    logicManager.stop();
    stopModules();
}

bsc::Node::Node()
    : Node({.rootPath = fs::path(std::getenv("HOME")) / ".bsc"}){LOGGER("default node constructor")}

      bsc::Node::Node(Node::Configuration configuration)
    : nodeConfiguration(std::move(configuration)) {
    LOGGER("configuration node constructor")
    nodeContext->set<NodeContext, Node&, NodeInfo&>(*this, this->thisNodeInfo);
    nodeContext->set<LoggerContext>([&] { return thisNodeInfo.getNodeId(); });
    nodeContext->setDirect<InputOutputContext>(std::make_shared<StandardInputOutputContext>());

    // this creates common context for role definitions across the entire node.
    nodeContext->set<RoleDefinitionsContext>();
    nodeContext->setDebugId("node contxt");

    nodeContext->set<FactoryContext>();

    logicManager.setContexts(nodeContext);

    Node::setNodeContextActive();
    LOGGER("node constructor finished")
}

void bsc::Node::startModules() {
    forEachModule<void>(&bsc::NodeModule::start);
    waitForModules([](const INodeModulePtr& modulePtr) {
        return modulePtr->isStarted();
    });//@todo wait for modules should take the same parameter as forEachModule for consistency, waitForModules(NodeModule::isStarted);
}

void bsc::Node::initialize() {
    setNodeContextActive();

    //    //@todo debug values
    configurationManager.setRootPath(nodeConfiguration.rootPath / this->getNodeInfo().getNodeId());

    // this slightly changed the order of execution - instead of being initialized and setupLogic module by module,
    // now all modules are initialized and then all modules are setupLogiced
    // hope it doesn't break anything
    forEachModule(&bsc::INodeModule::initializeConfiguration);
    forEachModule(&bsc::INodeModule::doInitialize);
    forEachModule(&bsc::INodeModule::doSetupLogic);
    forEachModule(&bsc::INodeModule::doPrepareSubmodules);
    forEachModule(&bsc::INodeModule::doReady);
}

void bsc::Node::stopModules() {
    forEachModule(&bsc::NodeModule::stop);
    waitForModules([](const INodeModulePtr& modulePtr) {
        return modulePtr->isFinished();
    });//@todo wait for modules should take the same parameter as forEachModule for consistency, waitForModules(NodeModule::isFinished);
}

void bsc::Node::joinModules() { forEachModule(&bsc::NodeModule::join); }

void bsc::Node::waitToFinish() {
    std::unique_lock g(startMutex);
    logicManager.join();
    joinModules();
}

void bsc::Node::shutdownModules() { forEachModule(&bsc::INodeModule::doShutdown); }

void bsc::Node::saveConfiguration() { forEachModule(&bsc::INodeModule::doSaveConfiguration); }

void bsc::Node::waitUntilStarted() {
    std::unique_lock g(startMutex);
    startedReady.wait(g, [this] { return started; });
}
