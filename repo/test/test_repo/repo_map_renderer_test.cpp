//
// Created by stilgar on 02.04.2020.
//


#include <catch2/catch.hpp>
#include <core/factory/FactoryContext.h>
#include <repo/repository/transformer/PathTransformerRuleFactory.h>
#include <repo/repository/transformer/rules/HomeDirRule.h>
#include <repo/repository/transformer/rules/TmpRule.h>

using namespace bsc;

JournalPtr prepareJournal() {
    const auto fileData = FileData{"", "hash", {}, 100, fs::file_time_type::min(), false};
    auto dirData = FileData{"", "hash", {}, 100, fs::file_time_type::min(), true};
    auto result = std::make_shared<SimpleJournal>(std::make_unique<FakeMetaDataFetcher>("node", "user", "system"));
    result->appendState<JournalTarget::file>({JournalMethod::add, bsc::fs::path("/tmp/dupa.txt"),fileData });
    result->appendState<JournalTarget::directory>({JournalMethod::add, bsc::fs::path("/tmp/dir"), dirData});
    result->appendState<JournalTarget::special>({JournalMethod::add, bsc::fs::path("/")});
    result->appendState<JournalTarget::transformer>({JournalMethod::add, PathTransformerRuleSelector::tmp});
    result->appendState<JournalTarget::transformer>({JournalMethod::add, PathTransformerRuleSelector::home});
    result->commitState(CommitTimeType::clock::now());
    return result;
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
    REQUIRE(fileMap.contains("/tmp/dir"));
    REQUIRE(fileMap.getPathTransformer().getRules().size() == 2);
    REQUIRE(fileMap.getPathTransformer().getRules().contains(std::make_shared<TmpRule>()));
    REQUIRE(fileMap.getPathTransformer().getRules().contains(std::make_shared<HomeDirRule>(getenv("HOME"))));


    //@todo implement all other cases (special etc.)
}