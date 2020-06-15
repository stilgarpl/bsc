//
// Created by stilgar on 02.04.2020.
//

#include "PathTransformerRuleFactory.h"
#include "PathTransformerRuleSelector.h"
#include <repo/repository/transformer/rules/CustomRule.h>
#include <repo/repository/transformer/rules/HomeDirRule.h>
#include <repo/repository/transformer/rules/TmpRule.h>

bsc::PathTransformerRulePtr bsc::PathTransformerRuleFactory::create(const SelectorType& selector,
                                                                    const ArgumentType& argument) const {
    switch (selector) {

        case PathTransformerRuleSelector::tmp:
            return std::make_shared<TmpRule>();
            break;
        case PathTransformerRuleSelector::home: {
            auto homeDir = getenv("HOME");
            //@todo if homeDir == null, throw exception
            return std::make_shared<HomeDirRule>(homeDir);
            break;
        }
        case PathTransformerRuleSelector::custom:
            //@todo assert argument.size == 2
            return std::make_shared<CustomRule>(argument[0],argument[1]);
            break;
    }
    return nullptr;
}
