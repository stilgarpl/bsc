//
// Created by stilgar on 12.11.17.
//

#include "Roles.h"
#include "RoleScope.h"

thread_local bsc::RoleScope* bsc::Roles::activeScope = nullptr;