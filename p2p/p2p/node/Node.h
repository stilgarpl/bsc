//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODE_H
#define BASYCO_NODE_H


#include "NodeInfo.h"

#include "p2p/node/configuration/IConfig.h"
#include "p2p/logic/SourceManager.h"
#include "p2p/logic/LogicManager.h"
#include "INode.h"

#include <memory>
#include <p2p/dependency/DependencyManager.h>
#include <p2p/role/RoleScope.h>
#include <p2p/node/configuration/ConfigurationManager.h>


class Node : public INode, public RoleScope {
public:
    typedef unsigned int IdType; //@todo replace it with a real id, hash or something

public:

    struct Configuration {
        fs::path rootPath;

        Configuration();
    };
private:

    ConfigurationManager configurationManager;
    Configuration nodeConfiguration;
    LogicManager logicManager;
    Context::Ptr nodeContext = Context::makeContext();
    //if somehow start is called from start or wait from start or stop from... change this to recursive mutex.
    std::mutex startMutex;
    std::condition_variable startedReady;
    bool started = false;
public:
    Context::Ptr getContext() override {
        return nodeContext;
    }

public:
    //@todo testing...
    void setNodeContextActive() override {
        Context::setActiveContext(nodeContext);
    }

private:


    NodeInfo thisNodeInfo;

public:

    ConfigurationManager &getConfigurationManager() override {
        return configurationManager;
    }

protected:


    void initialize();


public:


    void waitToFinish() override;

    void waitUntilStarted() override;

    void start() override;

    void stop() override;
    
    virtual ~Node();


    Node();

    Node(Node &) = delete;

    explicit Node(int port);

    LogicManager &getLogicManager() override {
        return logicManager;
    }

    NodeInfo &getNodeInfo() override {
        return thisNodeInfo;
    }

    friend class NodeActions;

    void startModules();

    void stopModules();

    void joinModules();

    void saveConfiguration() override;

protected:
    void shutdownModules();
};

#endif //BASYCO_NODE_H
