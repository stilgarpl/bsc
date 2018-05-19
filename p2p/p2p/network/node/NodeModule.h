//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_NODEMODULE_H
#define BASYCO_NODEMODULE_H


#include <p2p/logic/ILogicModule.h>
#include <p2p/dependency/IDependencyManaged.h>
#include <p2p/dependency/DependencyManaged.h>
#include "INode.h"
#include "INodeModule.h"

class NodeModule : public INodeModule {

protected:
    INode &node;
public:
    ///@todo pure or not?
    virtual void initialize() {};

    NodeModule(INode &node) : node(node) {}

    void run() override {
        //do nothing, just so modules do not have to implement this if they don't want to
    }
};

/**
 * NodeModule with automatic dependency management
 * @tparam T actual module type, i.e. class SomeModule : public NodeModuleDependent<SomeModule>
 * @tparam Args optional dependent classes. it could also be set by using setRequired<> method or addRequiredDependency<>
 */
template<typename T, typename ... Args>
class NodeModuleDependent : public NodeModule, public DependencyManaged<T> {

private:
//    template<typename T1, typename... Args1>
//    void checkAndAddModules() {
//        if (!node.hasModule<T1>()) {
//            node.addM
//        }
//    }

public:
    NodeModuleDependent(INode &node) : NodeModule(node) {
        this->template setRequired<Args...>();
    };
};


#endif //BASYCO_NODEMODULE_H
