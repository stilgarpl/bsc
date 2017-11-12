//
// Created by stilgar on 12.11.17.
//

#ifndef BASYCO_ROLES_H
#define BASYCO_ROLES_H


#include <p2p/log/Logger.h>
#include "Role.h"
#include "RoleInvalidException.h"
#include "RoleScope.h"

class Roles {

private:
    static thread_local RoleScope *activeScope;

public:

    static bool isRoleValid(const Role &role) {
        if (activeScope == nullptr) return false;

        return activeScope->findRole(role.getRoleId()) != nullptr;
    }

    template<typename ... Args>
    static void allowed(const Role::IdType &role, Args... args) {
        allowed(Role(role), args...);
    }

    template<typename ... Args>
    static void allowed(const Role &role, Args... args) {
        //@todo check if id is valid, if not throw exception
        LOGGER("Role is " + role.getRoleId());
        if (isRoleValid(role)) {
            allowed(args...);
        } else {
            throw RoleInvalidException();
        }
    }

    static void allowed() {

    }

    static RoleScope *getActiveScope() {
        return activeScope;
    }

    static void setActiveScope(RoleScope *activeScope) {
        Roles::activeScope = activeScope;
    }

};


#endif //BASYCO_ROLES_H
