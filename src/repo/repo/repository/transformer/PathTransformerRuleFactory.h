//
// Created by Krzysztof Tulidowicz on 02.04.2020.
//

#ifndef BSC_PATHTRANSFORMERRULEFACTORY_H
#define BSC_PATHTRANSFORMERRULEFACTORY_H

#include "PathTransformerRule.h"
#include "PathTransformerRuleSelector.h"
#include <core/factory/AutoFactory.h>

namespace bsc {

    class PathTransformerRuleFactory : public AutoFactory<PathTransformerRuleFactory, PathTransformerRulePtr> {
    public:
        [[nodiscard]] PathTransformerRulePtr create(const SelectorType& selector, const ArgumentType& argument) const override;
    };
}// namespace bsc

#endif