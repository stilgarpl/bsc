//
// Created by Krzysztof Tulidowicz on 12.11.17.
//

#ifndef BSC_ROLESCOPE_H
#define BSC_ROLESCOPE_H

#include <list>
#include <memory>
#include <algorithm>
#include "Role.h"
#include "RoleDefinitions.h"

namespace bsc {
    class RoleScope {
    private:

        RoleList roleList;
        RoleDefinitionsPtr definitions;

    public:


        void addRole(Role::IdType id) {
            roleList.push_back(std::make_shared<Role>(id));
        }

        RolePtr findRole(Role::IdType id) {
            auto result = std::find_if(roleList.begin(), roleList.end(), [&](RolePtr role) {
                return role->getRoleId() == id;
            });

            if (result != roleList.end()) {
                return *result;
            } else {
                return nullptr;
            }
        }


    };
}


#endif //BSC_ROLESCOPE_H
