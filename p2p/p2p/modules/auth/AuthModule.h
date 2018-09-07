//
// Created by stilgar on 19.07.18.
//

#ifndef BASYCO_AUTHMODULE_H
#define BASYCO_AUTHMODULE_H


#include <p2p/node/NodeModule.h>

class AuthModule : public NodeModuleDependent<AuthModule> {
public:
    AuthModule(INode &node);

    void setupActions(SetupActionHelper &actionHelper) override;

    bool assignActions(AssignActionHelper &actionHelper) override;

    bool setupSources(SetupSourceHelper &sourceHelper) override;

};


#endif //BASYCO_AUTHMODULE_H
