//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODE_H
#define BASYCO_NODE_H


#include "NodeInfo.h"

#include "p2p/node/configuration/IConfig.h"
#include "logic/SourceManager.h"
#include "logic/LogicManager.h"
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
    bsc::Context::Ptr nodeContext = bsc::Context::makeContext();
    //if somehow start is called from start or wait from start or stop from... change this to recursive mutex.
    std::recursive_mutex startMutex;
    std::condition_variable_any startedReady;
    bool started = false;
public:
    bsc::Context::Ptr getContext() override {
        return nodeContext;
    }

public:
    //@todo testing...
    void setNodeContextActive() override {
        bsc::Context::setActiveContext(nodeContext);
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

    Node(Node &&) = delete;

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
