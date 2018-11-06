//
// Created by stilgar on 04.11.18.
//

#include "PathTransformer.h"

std::filesystem::path PathTransformer::transformToJournalFormat(fs::path path) {
    for (const auto &rule : rules) {
        path = rule->transformToJournalFormat(path);
    }
    return path;
}

std::filesystem::path PathTransformer::transformFromJournalFormat(fs::path path) {
    for (auto rule = rules.rbegin(); rule != rules.rend(); rule++) {
        path = (*rule)->transformFromJournalFormat(path);
    }
    return path;
}

void PathTransformer::addRule(ITransformRulePtr rule) {
    rules.insert(rule);

}

void PathTransformer::removeRule(ITransformRulePtr rule) {
    rules.erase(rule);
}

IPathTransformer::RuleSet PathTransformer::getRules() {
    return rules;
}