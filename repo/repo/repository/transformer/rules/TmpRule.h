//
// Created by stilgar on 04.11.18.
//

#ifndef BSC_TMPRULE_H
#define BSC_TMPRULE_H


#include <repo/repository/transformer/PathTransformerRule.h>

namespace bsc {
    class TmpRule : public PathTransformerRule {
    private:
        const std::string TEMP_PATTERN = "<TEMP>";

    public:
        JournalPathType transformToJournalFormat(fs::path path) override;

        std::filesystem::path transformFromJournalFormat(JournalPathType path) override;

        ~TmpRule() override = default;

    };
}

#endif //BSC_TMPRULE_H
