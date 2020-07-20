//
// Created by Krzysztof Tulidowicz on 05.11.17.
//

#ifndef BSC_INODE_H
#define BSC_INODE_H


#include <p2p/core/dependency/DependencyManager.h>
#include <p2p/core/node/NodeInfo.h>
#include <core/context/Context.h>
#include <p2p/core/node/configuration/ConfigurationManager.h>
#include <logic/LogicManager.h>


namespace bsc {
    class INodeModule;

    template<typename ModuleType>
    using ModuleTypePtr = std::shared_ptr<ModuleType>;

    typedef ModuleTypePtr<INodeModule> INodeModulePtr;

    class INode {

    public:
        //is this necessary?
        virtual LogicManager& getLogicManager() = 0;

        virtual ConfigurationManager& getConfigurationManager() = 0;

        virtual void setNodeContextActive() = 0;


    private:
    protected:
        StaticUber<INodeModulePtr> modules;

        template<typename Ret, typename ModuleType, typename ... Args>
        void forEachModule(Ret(ModuleType::*f)(Args...), Args... args) {
            std::list<INodeModulePtr> modulesList;
            modules.forEach(
                    [&](const INodeModulePtr& ptr) {
                        if (ptr != nullptr) {
                            //ptr->initialize();
                            //ptr->setupLogic(logicManager);
                            modulesList.push_back(ptr);
                        };
                    });

            for (auto&& item : modulesList) {
                ((*item).*f)(args...);
            }
        }

        void waitForModules(const std::function<bool(INodeModulePtr)>& predicate) {
            modules.forEach([predicate](const INodeModulePtr& ptr) {
                while (!predicate(ptr)) {
                    //@todo C++20 wait on atomic - predicate should wait on atomic or return atomic to wait on.
                    std::this_thread::sleep_for(5ms);
                }
            });
        }

    public:
        template<typename ModuleType>
        auto addModule() {
            if (modules.get<ModuleType>() == nullptr) {
                modules.get<ModuleType>() = std::make_shared<ModuleType>(std::ref(*this));
            } else {
                LOGGER(std::string("Module ") + typeid(ModuleType).name() + " already added!")
            }
            return getModule<ModuleType>();
        }

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

        virtual NodeInfo& getNodeInfo() = 0;

        virtual Context::Ptr getContext() = 0;

        virtual void start() = 0;

        virtual void stop() = 0;

        virtual void waitToFinish() = 0;

        virtual void waitUntilStarted() = 0;


        virtual void saveConfiguration() = 0;

    };
}

#endif //BSC_INODE_H
