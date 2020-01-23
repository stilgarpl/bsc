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
        std::filesystem::path transformToJournalFormat(fs::path path) override;

        std::filesystem::path transformFromJournalFormat(fs::path path) override;

        ~HomeDirRule() override = default;

    };
}

#endif //BSC_HOMEDIRRULE_H
