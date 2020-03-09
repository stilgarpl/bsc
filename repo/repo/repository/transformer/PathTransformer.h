//
// Created by stilgar on 04.11.18.
//

#ifndef BSC_PATHTRANSFORMER_H
#define BSC_PATHTRANSFORMER_H


#include "IPathTransformer.h"

namespace bsc {
    class PathTransformer : public IPathTransformer {

        RuleSet rules;

    public:

    private:
        [[nodiscard]] JournalPathType transformToJournalFormat(fs::path path) const override;

        [[nodiscard]] std::filesystem::path transformFromJournalFormat(JournalPathType path) const override;

        void addRule(ITransformRulePtr rule) override;

        void removeRule(ITransformRulePtr rule) override;

        RuleSet getRules() override;

    public:
        ~PathTransformer() override = default;


    };
}

#endif //BSC_PATHTRANSFORMER_H
