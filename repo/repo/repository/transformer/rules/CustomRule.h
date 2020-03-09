//
// Created by stilgar on 04.11.18.
//

#ifndef BSC_CUSTOMRULE_H
#define BSC_CUSTOMRULE_H


#include <repo/repository/transformer/ITransformRule.h>

namespace bsc {
    class CustomRule : public ITransformRule {

    private:
        std::string from;
        std::string to;

    public:
        CustomRule(std::string from, std::string to);

        JournalPathType transformToJournalFormat(fs::path path) override;

        std::filesystem::path transformFromJournalFormat(JournalPathType path) override;

        ~CustomRule() override = default;
    };
}

#endif //BSC_CUSTOMRULE_H
