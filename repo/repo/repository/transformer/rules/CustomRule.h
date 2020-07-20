//
// Created by Krzysztof Tulidowicz on 04.11.18.
//

#ifndef BSC_CUSTOMRULE_H
#define BSC_CUSTOMRULE_H


#include <repo/repository/transformer/PathTransformerRule.h>

namespace bsc {
    class CustomRule : public PathTransformerRule {

    private:
        //@todo make it possible for user to set priority.
        constexpr static int customPriority = 20;
        std::string from;
        std::string to;

    public:
        CustomRule( std::string from, std::string to);

        JournalPathType transformToJournalFormat(fs::path path) override;

        std::filesystem::path transformFromJournalFormat(JournalPathType path) override;

    protected:
        std::string getDefinition() override;
    public:
        ~CustomRule() override = default;
    };
}

#endif //BSC_CUSTOMRULE_H
