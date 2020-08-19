//
// Created by Krzysztof Tulidowicz on 02.04.2020.
//

#include "FakeMetaDataFetcher.h"
#include <catch2/catch.hpp>
#include <core/context/Context.h>
#include <core/factory/FactoryContext.h>
#include <repo/journal/Journal.h>
#include <repo/journal/SimpleJournal.h>
#include <repo/repository/RepositoryFileMap.h>
#include <repo/repository/transformer/PathTransformerRuleFactory.h>
#include <repo/repository/transformer/rules/HomeDirRule.h>
#include <repo/repository/transformer/rules/TmpRule.h>

using namespace bsc;


JournalPtr prepareJournal() {
    const auto fileData = FileData{"", "fileHash", {}, 100, fs::file_time_type::min(), false};
    auto dirData        = FileData{"", "dirHash", {}, 100, fs::file_time_type::min(), true};
    auto result = std::make_shared<SimpleJournal>(std::make_unique<FakeMetaDataFetcher>("node", "user", "system"));
    result->appendState<JournalTarget::file>({JournalMethod::add, bsc::fs::path("/tmp/dupa.txt"), fileData});
    result->appendState<JournalTarget::directory>({JournalMethod::add, bsc::fs::path("/tmp/dir"), dirData});
    result->appendState<JournalTarget::transformer>({JournalMethod::add, PathTransformerRuleSelector::tmp});
    result->appendState<JournalTarget::transformer>({JournalMethod::add, PathTransformerRuleSelector::home});
    result->commitState(CommitTimeType::clock::now());
    return result;
}

void modifyJournal(const JournalPtr& journalPtr) {
    const auto fileData = FileData{"", "hash", {}, 100, fs::file_time_type::min(), false};
    journalPtr->appendState<JournalTarget::file>({JournalMethod::remove, bsc::fs::path("/tmp/dupa.txt"), fileData});
    journalPtr->appendState<JournalTarget::transformer>({JournalMethod::remove, PathTransformerRuleSelector::tmp});
    journalPtr->commitState(CommitTimeType::clock::now());
}

void setupContext() {
    Context::Ptr context = Context::makeContext();
    Context::setActiveContext(context);
    auto factoryContext = context->set<FactoryContext>();
    factoryContext->addFactory<PathTransformerRuleFactory>();
}

TEST_CASE("RepositoryFileMapRenderer test") {
    setupContext();
    RepositoryFileMapRenderer fileMapRenderer;
    auto journalPtr = prepareJournal();

    const auto& fileMap = fileMapRenderer.renderMap(journalPtr);
    REQUIRE(fileMap.contains("/tmp/dupa.txt"));
    REQUIRE(!fileMap.contains("/tmp/dupa.txt1"));
    REQUIRE(fileMap.contains("///tmp///dupa.txt"));
    REQUIRE(fileMap["/tmp/dupa.txt"]);
    REQUIRE(fileMap["/tmp/dupa.txt"]->getPermissions() == fs::perms());
    REQUIRE(!fileMap.isDeleted("/tmp/dupa.txt"));
    REQUIRE(fileMap.contains("/tmp/dir"));
    REQUIRE(fileMap.getPathTransformer().getRules().size() == 2);
    REQUIRE(fileMap.getPathTransformer().getRules().contains(std::make_shared<TmpRule>()));
    REQUIRE(fileMap.getPathTransformer().getRules().contains(std::make_shared<HomeDirRule>(getenv("HOME"))));

    modifyJournal(journalPtr);
    const auto& fileMap2 = fileMapRenderer.renderMap(journalPtr);
    REQUIRE(fileMap2.contains("/tmp/dupa.txt"));
    REQUIRE(fileMap2.isDeleted("/tmp/dupa.txt"));
    REQUIRE(!fileMap2["/tmp/dupa.txt"].has_value());
    REQUIRE(fileMap2.contains("/tmp/dir"));
    REQUIRE(fileMap2.getPathTransformer().getRules().size() == 1);
    REQUIRE(!fileMap2.getPathTransformer().getRules().contains(std::make_shared<TmpRule>()));
    REQUIRE(fileMap2.getPathTransformer().getRules().contains(std::make_shared<HomeDirRule>(getenv("HOME"))));
}