//
// Created by stilgar on 12.11.17.
//

#ifndef BASYCO_ROLESCOPE_H
#define BASYCO_ROLESCOPE_H

#include <list>
#include <memory>
#include <algorithm>
#include "Role.h"

class RoleScope {
private:

    std::list<std::shared_ptr<Role>> roleList;

public:


    void addRole(Role::IdType id) {
        roleList.push_back(std::make_shared<Role>(id));
    }

    std::shared_ptr<Role> findRole(Role::IdType id) {
        auto result = std::find_if(roleList.begin(), roleList.end(), [&](std::shared_ptr<Role> role) {
            return role->getRoleId() == id;
        });

        if (result != roleList.end()) {
            return *result;
        } else {
            return nullptr;
        }
    }


};


#endif //BASYCO_ROLESCOPE_H
