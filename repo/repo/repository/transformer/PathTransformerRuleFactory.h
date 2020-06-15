//
// Created by stilgar on 02.04.2020.
//

#ifndef BSC_PATHTRANSFORMERRULEFACTORY_H
#define BSC_PATHTRANSFORMERRULEFACTORY_H

#include "PathTransformerRule.h"
#include "PathTransformerRuleSelector.h"


namespace bsc {



    class PathTransformerRuleFactory : public Factory<PathTransformerRulePtr> {
    public:
        PathTransformerRulePtr create(const SelectorType& selector, const ArgumentType& argument) const override;
    };
}// namespace bsc

#endif