//
// Created by stilgar on 04.11.18.
//

#ifndef BSC_TMPRULE_H
#define BSC_TMPRULE_H


#include <repo/repository/transformer/ITransformRule.h>

namespace bsc {
    class TmpRule : public ITransformRule {
    private:
        const std::string TEMP_PATTERN = "<TEMP>";

    public:
        JournalPathType transformToJournalFormat(fs::path path) override;

        std::filesystem::path transformFromJournalFormat(JournalPathType path) override;

        ~TmpRule() override = default;

    };
}

#endif //BSC_TMPRULE_H
