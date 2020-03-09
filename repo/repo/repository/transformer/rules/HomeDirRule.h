//
// Created by stilgar on 04.11.18.
//

#ifndef BSC_HOMEDIRRULE_H
#define BSC_HOMEDIRRULE_H


#include <repo/repository/transformer/ITransformRule.h>

namespace bsc {
    class HomeDirRule : public ITransformRule {

    private:
        std::string HOME_DIR = getenv("HOME");
        std::string HOME_PATTERN = "<HOME>";

    public:
        JournalPathType transformToJournalFormat(fs::path path) override;

        std::filesystem::path transformFromJournalFormat(JournalPathType path) override;

        ~HomeDirRule() override = default;

    };
}

#endif //BSC_HOMEDIRRULE_H
