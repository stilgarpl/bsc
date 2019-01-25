//
// Created by stilgar on 19.07.18.
//

#ifndef BASYCO_AUTHMODULE_H
#define BASYCO_AUTHMODULE_H


#include <p2p/node/NodeModule.h>

class AuthModule : public NodeModuleDependent<AuthModule> {
public:

    class SubModule {
    public:
        //@todo add rules definition
        int a;
    };

    explicit AuthModule(INode &node);

    void setupActions(SetupActionHelper &actionHelper) override;

    bool assignActions(AssignActionHelper &actionHelper) override;

    bool setupSources(SetupSourceHelper &sourceHelper) override;

    void ready() override;

};


#endif //BASYCO_AUTHMODULE_H
