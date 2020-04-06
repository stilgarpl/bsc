//
// Created by stilgar on 04.11.18.
//

#ifndef BSC_PATHTRANSFORMERRULE_H
#define BSC_PATHTRANSFORMERRULE_H

#include <filesystem>
#include <set>

#include <repo/journal/JournalState.h>
#include <repo/journal/JournalStateData.h>
#include <core/factory/Factory.h>

namespace bsc {
    namespace fs = std::filesystem;
    using JournalPathType = std::remove_cvref_t<decltype(JournalStateData<JournalTarget::file>().getDestination())>;
    class PathTransformerRule {

    private:
        int priority = 10;
    public:
        virtual JournalPathType transformToJournalFormat(fs::path path) = 0;

        virtual fs::path transformFromJournalFormat(JournalPathType path) = 0;

        int getPriority() const {
            return priority;
        }

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
