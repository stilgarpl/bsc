//
// Created by stilgar on 07.04.2020.
//

#include "PathTransformer.h"

namespace bsc {

    fs::path PathTransformer::transformFromJournalFormat(JournalPathType path) const {
        for (auto rule = rules.rbegin(); rule != rules.rend(); rule++) {
            path = (*rule)->transformFromJournalFormat(path);
        }
        return path;
    }
    JournalPathType PathTransformer::transformToJournalFormat(fs::path path) const {
        for (const auto& rule : rules) {
            path = rule->transformToJournalFormat(path);
        }
        return path;
    }
}