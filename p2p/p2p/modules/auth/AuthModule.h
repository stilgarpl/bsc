//
// Created by stilgar on 19.07.18.
//

#ifndef BASYCO_AUTHMODULE_H
#define BASYCO_AUTHMODULE_H


#include <p2p/node/module/NodeModule.h>
#include <p2p/role/Roles.h>

class AuthModule : public NodeModuleDependent<AuthModule> {
public:

    class AuthRuleData {
    protected:
        std::set<Role::IdType> requiredRoles;

        void addRole(const Role::IdType &id) {
            requiredRoles.insert(id);
        }

        virtual void applyRule() = 0;

        friend class AuthModule;

        friend class SubModule;
        virtual ~AuthRuleData() = default;
    };

protected:
    template<typename PacketType>
    class SpecificAuthRuleData : public AuthRuleData {
    public:
        ~SpecificAuthRuleData() override = default;
    protected:
        void applyRule() override {
            for (const auto &item : requiredRoles) {
                LOGGER("APPLYING RULE")
                Roles::defineRequiredRole<PacketType>(item);
            }
        }
    };

public:
    class SubModule {
    private:
        std::list<std::shared_ptr<AuthRuleData>> rules;
    public:
        //@todo add rules definition
        int a;

        template<typename PacketType, typename ... IdTypes>
        void requiredRoles(const IdTypes... roleIds) {
            Role::IdType allIds[] = {roleIds...};
            auto r = std::make_shared<SpecificAuthRuleData<PacketType>>();
            for (const auto &i : allIds) {
                r->addRole(i);
            }
            rules.push_back(r);
        }

        void applyRules() {
            for (const auto &item : rules) {
                item->applyRule();
            }
        }
    };

    explicit AuthModule(INode &node);

    void setupActions(SetupActionHelper &actionHelper) override;

    bool assignActions(AssignActionHelper &actionHelper) override;

    bool setupSources(SetupSourceHelper &sourceHelper) override;

private:

    void prepareSubmodules() override;


};


#endif //BASYCO_AUTHMODULE_H
