//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_ITRANSFORMRULE_H
#define BASYCO_ITRANSFORMRULE_H

#include <filesystem>
#include <set>

namespace bsc {
    namespace fs = std::filesystem;

    class ITransformRule {

    private:
        int priority = 10;
    public:
        virtual fs::path transformToJournalFormat(fs::path path) = 0;

        virtual fs::path transformFromJournalFormat(fs::path path) = 0;

        int getPriority() const {
            return priority;
        }

        virtual ~ITransformRule() = default;
    };

    typedef std::shared_ptr<ITransformRule> ITransformRulePtr;
}
#endif //BASYCO_ITRANSFORMRULE_H
