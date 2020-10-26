//
// Created by Krzysztof Tulidowicz on 31.07.17.
//

#ifndef BSC_NODE_H
#define BSC_NODE_H


#include "p2p/core/node/configuration/IConfig.h"
#include "logic/SourceManager.h"
#include "logic/LogicManager.h"
#include "INode.h"
#include <memory>
#include <p2p/core/dependency/DependencyManager.h>
#include <p2p/core/role/RoleScope.h>
#include <p2p/core/node/configuration/ConfigurationManager.h>


namespace bsc {
    class Node : public INode, public RoleScope {
    public:
        typedef unsigned int IdType; //@todo replace it with a real id, hash or something

    public:

        struct Configuration {
            //@todo this rootPath is weird - it's a global root path for all nodes, where it should just be a path for this specific node.
            fs::path rootPath;
        };

    private:

        ConfigurationManager configurationManager;
        Configuration nodeConfiguration;
        LogicManager logicManager;
        Context::OwnPtr nodeContext = Context::makeContext();
        // if somehow start is called from start or wait from start or stop from... change this to recursive mutex.
        std::recursive_mutex startMutex;
        std::condition_variable_any startedReady;
        bool started = false;
    public:
        Context::Ptr getContext() override {
            return nodeContext;
        }

    public:
        void setNodeContextActive() override {
            Context::setActiveContext(nodeContext);
        }

    private:


        NodeInfo thisNodeInfo;

    public:

        ConfigurationManager& getConfigurationManager() override {
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

        Node(Configuration configuration);

        Node(Node&) = delete;

        Node(Node&&) = delete;

        LogicManager& getLogicManager() override {
            return logicManager;
        }

        NodeInfo& getNodeInfo() override {
            return thisNodeInfo;
        }

        friend class NodeActions;

        void startModules();

        void stopModules();

        void joinModules();

        void saveConfiguration() override;

        bool isRunning();

    protected:
        void shutdownModules();
    };
}

#endif //BSC_NODE_H
