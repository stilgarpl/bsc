//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_HOMEDIRRULE_H
#define BASYCO_HOMEDIRRULE_H


#include <repo/repository/transformer/ITransformRule.h>

class HomeDirRule : public ITransformRule {

private:
    std::string HOME_DIR = getenv("HOME");
    std::string HOME_PATTERN = "<HOME>";

public:
    std::filesystem::path transformToJournalFormat(fs::path path) override;

    std::filesystem::path transformFromJournalFormat(fs::path path) override;

    ~HomeDirRule() override = default;

};


#endif //BASYCO_HOMEDIRRULE_H
