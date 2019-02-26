//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_CUSTOMRULE_H
#define BASYCO_CUSTOMRULE_H


#include <repo/repository/transformer/ITransformRule.h>

class CustomRule : public ITransformRule {

private:
    std::string from;
    std::string to;

public:
    CustomRule(const std::string &from, const std::string &to);

    std::filesystem::path transformToJournalFormat(fs::path path) override;

    std::filesystem::path transformFromJournalFormat(fs::path path) override;
    ~CustomRule() override = default;
};


#endif //BASYCO_CUSTOMRULE_H
