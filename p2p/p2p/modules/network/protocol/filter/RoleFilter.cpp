//
// Created by stilgar on 16.07.18.
//

#include "RoleFilter.h"

bool bsc::RoleFilter::filter(BasePacketPtr packet) {

    auto requiredRoles = packet->requiredRoles();
    bool roleValid = true;
    for (auto&& item : requiredRoles) {
        roleValid &= Roles::isRoleValid(*item);
    }
    return roleValid;
//    packet
}

