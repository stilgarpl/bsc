//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_PATHTRANSFORMER_H
#define BASYCO_PATHTRANSFORMER_H


#include "IPathTransformer.h"
namespace bsc {
    class PathTransformer : public IPathTransformer {

        RuleSet rules;

    public:

    private:
        [[nodiscard]] std::filesystem::path transformToJournalFormat(fs::path path) const override;

        [[nodiscard]] std::filesystem::path transformFromJournalFormat(fs::path path) const override;

        void addRule(ITransformRulePtr rule) override;

        void removeRule(ITransformRulePtr rule) override;

        RuleSet getRules() override;

    public:
        ~PathTransformer() override = default;


    };
}

#endif //BASYCO_PATHTRANSFORMER_H
