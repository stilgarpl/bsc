//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_IPATHTRANSFORMER_H
#define BASYCO_IPATHTRANSFORMER_H

#include <filesystem>
#include <set>
#include "ITransformRule.h"


namespace fs = std::filesystem;

class IPathTransformer {
private:


    class TransformComparator {
    public:
        bool operator()(const ITransformRulePtr &left, const ITransformRulePtr &right) const {
            return left->getPriority() < right->getPriority();
        }

    };

public:
    typedef std::multiset<ITransformRulePtr, TransformComparator> RuleSet;

public:

    virtual fs::path transformToJournalFormat(fs::path path) const = 0;

    virtual fs::path transformFromJournalFormat(fs::path path) const = 0;

    //@todo probably shared_ptrs would be better
    virtual void addRule(ITransformRulePtr rule) = 0;

    virtual void removeRule(ITransformRulePtr rule) = 0;

    virtual RuleSet getRules() = 0;

    virtual ~IPathTransformer() = default;
};


#endif //BASYCO_IPATHTRANSFORMER_H
