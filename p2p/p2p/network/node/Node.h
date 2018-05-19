//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODE_H
#define BASYCO_NODE_H


#include "NodeInfo.h"

#include "NetworkInfo.h"

#include "p2p/modules/configuration/IConfig.h"
#include "p2p/logic/SourceManager.h"
#include "p2p/logic/LogicManager.h"
#include "NodeModule.h"
#include "INode.h"

#include <memory>
#include <p2p/network/protocol/connection/Connection.h>
#include <p2p/network/protocol/protocol/IProtocol.h>
#include <p2p/network/protocol/protocol/GravitonProtocol.h>
#include <p2p/network/protocol/connection/IServerConnection.h>
#include <p2p/dependency/DependencyManager.h>
#include <p2p/network/protocol/role/RoleScope.h>

///@todo separate interface so NodeInfo can include INode, and Node can include NodeInfo

class NodeActions;



class Node : public INode, public RoleScope {
public:
    typedef unsigned int IdType; ///@todo replace it with a real id, hash or something

public:
//    class Config : public IConfig {
//    private:
//        unsigned short port;
//    private:
//        template<class Archive>
//        void serialize(Archive &ar) {
//            ar & cereal::base_class<IConfig>(this);
//            ar & port;
//        }
//
//
//        friend class cereal::access;
//
//    public:
//        unsigned short getPort() const {
//            return port;
//        }
//
//        void setPort(unsigned short port) {
//            Config::port = port;
//        }
//    };

private:

//    std::shared_ptr<Config> configuration;
    LogicManager logicManager;
    Context nodeContext;

    virtual Context &getContext() {
        return nodeContext;
    }

public:
    ///@todo testing...
    void setNodeContextActive() {
        Context::setActiveContext(&nodeContext);
    }

public:
//    const std::shared_ptr<Config> &getConfiguration() const;
//
//    void setConfiguration(const std::shared_ptr<Config> &configuration);

private:


    NodeInfo thisNodeInfo;
    std::shared_ptr<NetworkInfo> networkInfo;// = nsm(networkInfo); //network this node belongs to @todo more than 1?


    // typedef std::shared_ptr<INodeModule> INodeModulePtr;
    StaticUber<INodeModulePtr> modules;



public:

    template<typename ModuleType>
    void addModule() {
        ///@todo check if module exist before overwriting?
        modules.get<ModuleType>() = std::make_shared<ModuleType>(std::ref(*this));
    }

//    template<typename ModuleType>
//    ModuleTypePtr getModulePtr() {
//        return modules.get<ModuleType>();
//    }


    template<typename ModuleType>
    bool hasModule() {
        return modules.get<ModuleType>() != nullptr;
        //return modules.get<ModuleType>();

    }


    template<typename ModuleType>
    ModuleTypePtr<ModuleType> getModule() {
        return std::static_pointer_cast<ModuleType>(modules.get<ModuleType>());
        //return modules.get<ModuleType>();

    }



protected:


    void work();

    void initialize();;


public:


    void waitToFinish() override;

    void start();
    void stop();
    
    virtual ~Node();


    Node();

    Node(int port);

    ///@todo remove this function
    LogicManager &getLogicManager() {
        return logicManager;
    }

    virtual NodeInfo &getNodeInfo() {
        return thisNodeInfo;
    }

    void addToNetwork(NetworkIdType networkId) {
        networkInfo = std::make_shared<NetworkInfo>();
        networkInfo->setNetworkId(networkId);
        ///@todo this shouldn't be set twice... or should it?
        thisNodeInfo.setNetworkId(networkId);
    }

    std::shared_ptr<NetworkInfo> &getNetworkInfo();

    ///@todo move those random methods somewhere else:



    friend class NodeActions;

    void startModules();

    void stopModules();

    void joinModules();

protected:
    template<typename Ret, typename ModuleType, typename ... Args>
    void forEachModule(Ret(ModuleType::*f)(Args...), Args... args) {
        std::list<INodeModulePtr> modulesList;
        modules.forEach(
                [&](INodeModulePtr ptr) {
                    if (ptr != nullptr) {
                        //ptr->initialize();
                        //ptr->setupLogic(logicManager);
                        modulesList.push_back(ptr);
                    };
                });
        auto sortedList = DependencyManager::dependencySort(modulesList);

        for (auto &&item : sortedList) {
            ((*item).*f)(args...);
        }
    }

    INodeModulePtr getModuleByDependencyId(DependencyManager::TypeIdType id) override;
};

//CEREAL_REGISTER_TYPE(Node::Config)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(IConfig, Node::Config)

#endif //BASYCO_NODE_H
