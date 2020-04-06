//
// Created by stilgar on 07.12.17.
//
#include <catch2/catch.hpp>
#include <fstream>
#include <repo/journal/SimpleJournal.h>
#include <repo/journal/SimpleJournalMetaDataFetcher.h>

using namespace bsc;
class FakeMetaDataFetcher : public JournalMetaDataFetcher {
    JournalMetaData metaData;
public:
    JournalMetaData makeMetaData() override {
        return metaData;
    }

    FakeMetaDataFetcher(std::string nodeId,
                        std::string userId,
                        std::string operatingSystemId) : metaData(nodeId, userId, operatingSystemId) {

    }


};


TEST_CASE("Journal deterministic hash test") {
    SimpleJournal journal(std::make_unique<FakeMetaDataFetcher>("node", "user", "system"));
    journal.append(JournalMethod::add, JournalTarget::file, "/tmp/dupa.txt",
                   bsc::FileData("/tmp/dupa.txt", "hash", {}, 100, fs::file_time_type::min(), false));
    journal.commitState(CommitTimeType::clock::from_time_t(0));
    journal.append(JournalMethod::add, JournalTarget::file, "/tmp/other.txt",
                   bsc::FileData("/tmp/other.txt", "hash2", {}, 100, fs::file_time_type::min(), false));
    journal.commitState(CommitTimeType::clock::from_time_t(0));
    REQUIRE_THAT(journal.getChecksum(), Catch::Matchers::Equals("5025f3f7d793c9e8f82946a246c395e30aa6a248"));
}


TEST_CASE("Journal merge test") {
    SimpleJournal journal;
    journal.append(JournalMethod::add, JournalTarget::file, "/tmp/THIS_IS_OTHER_TEST.txt",
                   bsc::FileData("/tmp/THIS_IS_OTHER_TEST.txt"));
    journal.append(JournalMethod::remove, JournalTarget::file, "/tmp/to_remove.txt",
                   bsc::FileData("/tmp/to_remove.txt"));
    journal.append(JournalMethod::remove, JournalTarget::directory, "/tmp/folder/",
                   bsc::FileData("/tmp/folder/","llll",{},{},{},true));
    journal.commitState(CommitTimeType::clock::now());
    journal.append(JournalMethod::modify, JournalTarget::file, "/tmp/dupa.txt", bsc::FileData("/tmp/to_remove.txt"));
    JournalFuncMap journalFuncMap;
    journal.replay(journalFuncMap);
    journal.commitState(CommitTimeType::clock::now());
    {
        std::ofstream os("/tmp/journal.xml");
        cereal::XMLOutputArchive oa(os);
        oa << journal;
    }

    auto firstChecksum = journal.calculateChecksum();


    SECTION("ok merge") {

        std::shared_ptr<SimpleJournal> journal1 = std::make_shared<SimpleJournal>();
        {
            std::ifstream is("/tmp/journal.xml");
            cereal::XMLInputArchive ia(is);
            ia >> *journal1;
        }


        journal1->append(JournalMethod::add, JournalTarget::file, "/bin/zsh", bsc::FileData("/bin/zsh"));
        journal1->commitState(CommitTimeType::clock::now());

        REQUIRE(journal.merge(journal1));
        auto checksum = journal.calculateChecksum();
        REQUIRE(checksum != firstChecksum);

    }

    SECTION("fail merge") {

        std::shared_ptr<SimpleJournal> journal2 = std::make_shared<SimpleJournal>();
        journal2->append(JournalMethod::add, JournalTarget::file, "/tmp/journal.xml",
                         bsc::FileData("/tmp/journal.xml"));
        journal2->commitState(CommitTimeType::clock::now());


        REQUIRE_FALSE(journal.merge(journal2));
        auto checksum = journal.calculateChecksum();
        REQUIRE(checksum == firstChecksum);
    }

    //cleanup

    fs::remove("/tmp/journal.xml");


}