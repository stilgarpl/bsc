//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODE_H
#define BASYCO_NODE_H


#include "NodeInfo.h"

#include "p2p/modules/configuration/IConfig.h"
#include "p2p/logic/SourceManager.h"
#include "p2p/logic/LogicManager.h"
#include "INode.h"

#include <memory>
#include <p2p/dependency/DependencyManager.h>
#include <p2p/role/RoleScope.h>
#include <p2p/modules/configuration/ConfigurationManager.h>


class Node : public INode, public RoleScope {
public:
    typedef unsigned int IdType; //@todo replace it with a real id, hash or something

public:
private:

    ConfigurationManager configurationManager;
    LogicManager logicManager;
    Context::Ptr nodeContext = Context::makeContext();

    Context::Ptr getContext() override {
        return nodeContext;
    }

public:
    //@todo testing...
    void setNodeContextActive() override {
        Context::setActiveContext(nodeContext);
    }

public:
//    const std::shared_ptr<Config> &getConfiguration() const;
//
//    void setConfiguration(const std::shared_ptr<Config> &configuration);

private:


    NodeInfo thisNodeInfo;
    // = nsm(networkInfo); //network this node belongs to @todo more than 1?


    // typedef std::shared_ptr<INodeModule> INodeModulePtr;




public:

    ConfigurationManager &getConfigurationManager() override {
        return configurationManager;
    }



protected:


    void work();

    void initialize();


public:


    void waitToFinish() override;

    void start() override;

    void stop() override;
    
    virtual ~Node();


    Node();

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

protected:


//    template<typename Ret, typename ... Args>
//    void forEachModule(std::function<Ret(Args...)> f, Args... args) {
//        std::list<INodeModulePtr> modulesList;
//        modules.forEach(
//                [&](INodeModulePtr ptr) {
//                    if (ptr != nullptr) {
//                        //ptr->initialize();
//                        //ptr->setupLogic(logicManager);
//                        modulesList.push_back(ptr);
//                    };
//                });
//        auto sortedList = DependencyManager::dependencySort(modulesList);
//
//        for (auto &&item : sortedList) {
//            ((*item).*f)(args...);
//        }
//    }

//    friend class NodeModule;
protected:
    void shutdownModules();
};

//CEREAL_REGISTER_TYPE(Node::Config)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(IConfig, Node::Config)

#endif //BASYCO_NODE_H
