//
// Created by Krzysztof Tulidowicz on 04.11.18.
//

#ifndef BSC_PATHTRANSFORMER_H
#define BSC_PATHTRANSFORMER_H

#include "PathTransformerRule.h"
#include <filesystem>
#include <set>

namespace bsc {
    namespace fs = std::filesystem;

    class PathTransformer {
     private:

        class TransformComparator {
        public:
            bool operator()(const PathTransformerRulePtr& left, const PathTransformerRulePtr& right) const {
                //@todo perfect place to use <=>
                auto priority = left->getPriority() - right->getPriority();
                if (priority == 0) {
                    return left->getDefinition() < right->getDefinition();
                } else {
                    return priority < 0;
                }
            }

        };
        typedef std::multiset<PathTransformerRulePtr, TransformComparator> RuleSet;
        RuleSet rules;
    public:

        [[nodiscard]] virtual JournalPathType transformToJournalFormat(fs::path path) const;

        [[nodiscard]] virtual fs::path transformFromJournalFormat(JournalPathType path) const;

         void addRule(PathTransformerRulePtr rule) {
            rules.insert(rule);

        }

         void removeRule(PathTransformerRulePtr rule) {
            rules.erase(rule);
        }


         const RuleSet& getRules() const {
            return rules;
        }

        virtual ~PathTransformer() = default;
    };
}

#endif//BSC_PATHTRANSFORMER_H
