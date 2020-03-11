//
// Created by stilgar on 04.11.18.
//

#ifndef BSC_ITRANSFORMRULE_H
#define BSC_ITRANSFORMRULE_H

#include <filesystem>
#include <set>

#include <repo/journal/JournalState.h>

namespace bsc {
    namespace fs = std::filesystem;
    using JournalPathType = std::remove_cvref_t<decltype(JournalStateData<JournalTarget::file>().getDestination())>;
    class ITransformRule {

    private:
        int priority = 10;
    public:
        virtual JournalPathType transformToJournalFormat(fs::path path) = 0;

        virtual fs::path transformFromJournalFormat(JournalPathType path) = 0;

        int getPriority() const {
            return priority;
        }

        virtual ~ITransformRule() = default;
    };

    typedef std::shared_ptr<ITransformRule> ITransformRulePtr;
}
#endif //BSC_ITRANSFORMRULE_H
