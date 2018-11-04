//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_TMPRULE_H
#define BASYCO_TMPRULE_H


#include <repo/repository/transformer/ITransformRule.h>

class TmpRule : public ITransformRule {
private:
    const std::string TEMP_PATTERN = "<TEMP>";

public:
    std::filesystem::path transformToJournalFormat(fs::path path) override;

    std::filesystem::path transformFromJournalFormat(fs::path path) override;

};


#endif //BASYCO_TMPRULE_H
