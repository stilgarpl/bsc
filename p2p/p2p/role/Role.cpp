//
// Created by stilgar on 12.11.17.
//

#include "Role.h"

Role::Role(const Role::IdType &roleId) : roleId(roleId) {}

const Role::IdType &Role::getRoleId() const {
    return roleId;
}

bool Role::operator==(const Role &rhs) const {
    return roleId == rhs.roleId;
}

bool Role::operator!=(const Role &rhs) const {
    return !(rhs == *this);
}
