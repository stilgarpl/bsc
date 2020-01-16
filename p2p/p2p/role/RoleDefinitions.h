//
// Created by stilgar on 16.07.18.
//

#ifndef BASYCO_ROLEDEFINITIONS_H
#define BASYCO_ROLEDEFINITIONS_H


#include <core/uber/Uber.h>
#include <core/log/Logger.h>
#include "Role.h"


namespace bsc {
    class RoleDefinitions {

    private:
        StaticUber<RoleList> requiredRoles;

    public:

        template<typename T>
        const RoleList& getRequiredRoles() {
            return requiredRoles.get<T>();
        }

        template<typename T>
        void addRole(const Role::IdType& roleId) {
            //@todo check if role is already present
            requiredRoles.get<T>().push_back(Role::makeRole(roleId));
        }
    };

    typedef std::shared_ptr<RoleDefinitions> RoleDefinitionsPtr;

    class RoleDefinitionsContext {
    public:
        RoleDefinitionsPtr roleDefinitionsPtr = std::make_shared<RoleDefinitions>();
    };

}


#endif //BASYCO_ROLEDEFINITIONS_H
