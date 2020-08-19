//
// Created by Krzysztof Tulidowicz on 04.11.18.
//

#ifndef BSC_HOMEDIRRULE_H
#define BSC_HOMEDIRRULE_H


#include <repo/repository/transformer/PathTransformerRule.h>

namespace bsc {
    class HomeDirRule : public PathTransformerRule {

    private:
        constexpr static int homeDirPriority = 99999;
        std::string homeDir;
        const std::string homePattern = "<HOME>";

    public:
        JournalPathType transformToJournalFormat(fs::path path) override;

        std::filesystem::path transformFromJournalFormat(JournalPathType path) override;

        ~HomeDirRule() override = default;

    protected:
        std::string getDefinition() override;
    public:
        HomeDirRule(const std::string& homeDir);
    };
}

#endif //BSC_HOMEDIRRULE_H
