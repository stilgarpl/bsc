//
// Created by stilgar on 04.11.18.
//

#ifndef BSC_PATHTRANSFORMERRULE_H
#define BSC_PATHTRANSFORMERRULE_H

#include <filesystem>
#include <set>

#include <core/factory/Factory.h>
#include <repo/journal/JournalState.h>
#include <repo/journal/JournalStateEntry.h>

namespace bsc {
    namespace fs = std::filesystem;
    using JournalPathType = std::remove_cvref_t<decltype(JournalStateEntry<JournalTarget::file>().getDestination())>;

    class PathTransformerRule {

    private:
        int priority = 10;
    public:
        virtual JournalPathType transformToJournalFormat(fs::path path) = 0;

        virtual fs::path transformFromJournalFormat(JournalPathType path) = 0;

        int getPriority() const {
            return priority;
        }
        //@todo getDefinition should be protected, but Comparator needs to see it.
        //@todo maybe think of better name. it is used to compare rules with the same priority
        virtual std::string getDefinition() {
            return "";
        }
        PathTransformerRule(int priority) : priority(priority) {}

        virtual ~PathTransformerRule() = default;
    };

    typedef std::shared_ptr<PathTransformerRule> PathTransformerRulePtr;
    template<>
    class FactoryTraits<PathTransformerRulePtr> {
    public:
        using SelectorType = PathTransformerRuleSelector;
        using ArgumentType = std::vector<std::string>;
    };
}
#endif//BSC_PATHTRANSFORMERRULE_H
