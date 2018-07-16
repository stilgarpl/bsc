//
// Created by stilgar on 12.11.17.
//

#ifndef BASYCO_ROLE_H
#define BASYCO_ROLE_H

#include <string>
#include <memory>
#include <list>

class Role;

typedef std::shared_ptr<Role> RolePtr;
typedef std::list<RolePtr> RoleList;

class Role {
public:
    typedef std::string IdType;

private:

    IdType roleId;

public:
    Role(const IdType &roleId);

    const IdType &getRoleId() const;

    static RolePtr makeRole(const IdType &id) {
        return std::make_shared<Role>(id);
    }

    bool operator==(const Role &rhs) const;

    bool operator!=(const Role &rhs) const;
};


/**
 * ideas.... maybe inherit from Role
 * and make Uber Role manager
 * if (manager.hasRole<UserRole>()) ... but how to get UserRole from packet?
 *
 * use the trick from dependency management?
 *
 * role = Role<User,Admin,Other> ?
 * or ...
 * role = role(Role::User, Role::Admin, Role::Other) ?
 *
 * if if (manager.hasRoles(packet->getRoles())
 *
 *
 * Roles::Allowed("Admin","User");
 *
 * Roles::Allowed(Role::Admin, Role::User)
 * Roles::Allowed(Role::Admin) -> throws exception on wrong role
 *
 *
 * RoleScope
 *
 * Node is RoleScope
 * Connection is RoleScope
 *
 * roles are stored in rolescope
 *
 * thread_local current_role_scope?
 *
 * createRole(Role, Scope)
 *
 * Roles::createRole("admin",connection);
 *
 * if currentRoleScope == nullptr, throw NoScopeException
 *
 * what about mixed scopes?
 *
 * Roles::allowed("ConnectionScopeAdmin","NodeScopeDupa") ?
 *
 * Roles::allowed(connection, "admin","user");
 * Roles::allowed(node, "dupa"); ?
 *
 */


#endif //BASYCO_ROLE_H
