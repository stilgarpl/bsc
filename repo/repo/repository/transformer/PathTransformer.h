//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_PATHTRANSFORMER_H
#define BASYCO_PATHTRANSFORMER_H


#include "IPathTransformer.h"

class PathTransformer : public IPathTransformer {

    RuleSet rules;

public:

private:
    std::filesystem::path transformToJournalFormat(fs::path path) override;

    std::filesystem::path transformFromJournalFormat(fs::path path) override;

    void addRule(ITransformRulePtr rule) override;

    void removeRule(ITransformRulePtr rule) override;

    RuleSet getRules() override;
public:
    ~PathTransformer() override = default;


};


#endif //BASYCO_PATHTRANSFORMER_H
