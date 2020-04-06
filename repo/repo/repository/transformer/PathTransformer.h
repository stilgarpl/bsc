//
// Created by stilgar on 04.11.18.
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
                return left->getPriority() < right->getPriority();
            }

        };
        typedef std::multiset<PathTransformerRulePtr, TransformComparator> RuleSet;
        RuleSet rules;
    public:

        [[nodiscard]] virtual JournalPathType transformToJournalFormat(fs::path path) const {
            for (const auto& rule : rules) {
                path = rule->transformToJournalFormat(path);
            }
            return path;
        }

        [[nodiscard]] virtual fs::path transformFromJournalFormat(JournalPathType path) const {
            for (auto rule = rules.rbegin(); rule != rules.rend(); rule++) {
                path = (*rule)->transformFromJournalFormat(path);
            }
            return path;
        }

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
