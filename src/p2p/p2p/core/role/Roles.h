//
// Created by Krzysztof Tulidowicz on 12.11.17.
//

#ifndef BSC_ROLES_H
#define BSC_ROLES_H

#include "Role.h"
#include "RoleInvalidException.h"
#include "RoleScope.h"
#include <context/context/Context.h>
#include <log/log/Logger.h>

namespace bsc {
    class Roles {

    private:
        static thread_local RoleScope* activeScope;

    public:

        static bool isRoleValid(const Role& role) {
            if (activeScope == nullptr) return false;

            return activeScope->findRole(role.getRoleId()) != nullptr;
        }

        template<typename ... Args>
        static void allowed(const Role::IdType& role, Args... args) {
            allowed(Role(role), args...);
        }

        template<typename ... Args>
        static void allowed(const Role& role, Args... args) {
            //@todo check if id is valid, if not throw exception
            LOGGER("Role is " + role.getRoleId());
            if (isRoleValid(role)) {
                allowed(args...);
            } else {
                throw RoleInvalidException("No such role");
            }
        }

        static void allowed() {

        }

        static RoleScope* getActiveScope() {
            return activeScope;
        }

        static void setActiveScope(RoleScope* activeScope) {
            Roles::activeScope = activeScope;
        }

        template<typename T>
        static void defineRequiredRole(const Role::IdType& roleId) {
            //@todo nullptr check?
            Context::getActiveContext()->get<RoleDefinitionsContext>()->roleDefinitionsPtr->addRole<T>(roleId);
        }

        template<typename T>
        static const RoleList& getRequiredRolesDefinitions() {
            return Context::getActiveContext()->get<RoleDefinitionsContext>()->roleDefinitionsPtr->getRequiredRoles<T>();
        }

    };
}


#endif //BSC_ROLES_H
