//
// Created by stilgar on 05.11.17.
//

#ifndef BASYCO_INODE_H
#define BASYCO_INODE_H


#include <p2p/dependency/DependencyManager.h>
#include <p2p/node/NodeInfo.h>
#include <p2p/context/Context.h>
#include <p2p/modules/configuration/ConfigurationManager.h>
#include <p2p/logic/LogicManager.h>

class INodeModule;

template<typename ModuleType>
using ModuleTypePtr = std::shared_ptr<ModuleType>;

typedef ModuleTypePtr<INodeModule> INodeModulePtr;

class INode {
protected:
    std::mutex lock;
public:
    //is this necessary?
    virtual LogicManager &getLogicManager() =0;

    virtual ConfigurationManager &getConfigurationManager() =0;

    virtual void setNodeContextActive() =0;

//    virtual void runModuleMod(std::function<void()> method,INodeModulePtr module) = 0;

private:
    std::mutex &getLock() {
        return lock;
    }

private:
protected:
    StaticUber<INodeModulePtr> modules;

public:

    template<typename ModuleType>
    void addModule() {
        //@todo check if module exist before overwriting?
        if (modules.get<ModuleType>() == nullptr) {
            modules.get<ModuleType>() = std::make_shared<ModuleType>(std::ref(*this));
        } else {
            LOGGER(std::string("Module ") + typeid(ModuleType).name() + " already added!")
        }
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

//    template<typename ModuleType>
//    ModuleTypePtr<ModuleType> getModule() {
//        //  std::lock_guard<std::mutex> g(lock);
//        auto id = DependencyManager::getClassId<ModuleType>();
//        //possible to cast, we know exactly what type this pointer has (it has either good type or it's null
//        return std::static_pointer_cast<ModuleType>(getModuleByDependencyId(id));
//    }
//
//    template<typename ModuleType>
//    bool hasModule() {
//        return getModule<ModuleType>() != nullptr;
//
////        runModuleMod([&](){}, nullptr);
//    }

    virtual NodeInfo &getNodeInfo() =0;

    virtual Context::Ptr getContext() =0;

    virtual void start()=0;

    virtual void stop()=0;

    virtual void waitToFinish() = 0;

};

#endif //BASYCO_INODE_H
