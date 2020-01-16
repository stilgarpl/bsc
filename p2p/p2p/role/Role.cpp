//
// Created by stilgar on 12.11.17.
//

#include "Role.h"

bsc::Role::Role(const Role::IdType& roleId) : roleId(roleId) {}

const bsc::Role::IdType& bsc::Role::getRoleId() const {
    return roleId;
}

bool bsc::Role::operator==(const bsc::Role& rhs) const {
    return roleId == rhs.roleId;
}

bool bsc::Role::operator!=(const bsc::Role& rhs) const {
    return !(rhs == *this);
}
