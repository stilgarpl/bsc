//
// Created by stilgar on 16.07.18.
//

#ifndef BASYCO_ROLEDEFINITIONS_H
#define BASYCO_ROLEDEFINITIONS_H


#include <p2p/uber/Uber.h>
#include <p2p/log/Logger.h>
#include "Role.h"

class RoleDefinitions {

private:
    StaticUber<RoleList> requiredRoles;

public:

    RoleDefinitions() {
        //@todo remove
        LOGGER("ROLE DEFINITIONS CONSTRUCTOR");
    }

    template<typename T>
    const RoleList &getRequiredRoles() {
        return requiredRoles.get<T>();
    }

    template<typename T>
    void addRole(const Role::IdType &roleId) {
        //@todo check if role is already present
        requiredRoles.get<T>().push_back(Role::makeRole(roleId));
    }
};

typedef std::shared_ptr<RoleDefinitions> RoleDefinitionsPtr;

class RoleDefinitionsContext {
public:
    RoleDefinitionsPtr roleDefinitionsPtr = std::make_shared<RoleDefinitions>();
};


#endif //BASYCO_ROLEDEFINITIONS_H
