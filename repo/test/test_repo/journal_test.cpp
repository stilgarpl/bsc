//
// Created by stilgar on 07.12.17.
//
#include <catch2/catch.hpp>
#include <repo/journal/SimpleJournal.h>
#include <fstream>

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
    journal.append(JournalMethod::ADDED, JournalTarget::FILE, "/tmp/dupa.txt",
                   FileData("/tmp/dupa.txt", "hash", {}, 100, fs::file_time_type::min(), false));
    journal.commitState(CommitTimeType::clock::from_time_t(0));
    REQUIRE_THAT(journal.getChecksum(), Catch::Matchers::Equals("426675e4107b908d73623485eaf76ec0e6c0a022"));
}


TEST_CASE("Journal merge test") {
    SimpleJournal journal;
    journal.append(JournalMethod::ADDED, JournalTarget::FILE, "/tmp/THIS_IS_OTHER_TEST.txt",
                   FileData("/tmp/THIS_IS_OTHER_TEST.txt"));
    journal.append(JournalMethod::DELETED, JournalTarget::FILE, "/tmp/to_remove.txt", FileData("/tmp/to_remove.txt"));
    journal.commitState(CommitTimeType::clock::now());
    journal.append(JournalMethod::MODIFIED, JournalTarget::FILE, "/tmp/dupa.txt", FileData("/tmp/to_remove.txt"));
    journal.replay();
    journal.commitState(CommitTimeType::clock::now());
    {
        std::ofstream os("/tmp/journal.xml");
        cereal::XMLOutputArchive oa(os);
        oa << journal;
    }


    SECTION("ok merge") {

        std::shared_ptr<SimpleJournal> journal1 = std::make_shared<SimpleJournal>();
        {
            std::ifstream is("/tmp/journal.xml");
            cereal::XMLInputArchive ia(is);
            ia >> *journal1;
        }


        journal1->append(JournalMethod::ADDED, JournalTarget::FILE, "/bin/zsh", FileData("/bin/zsh"));
        journal1->commitState(CommitTimeType::clock::now());


        REQUIRE(journal.merge(journal1));

    }

    SECTION("fail merge") {

        std::shared_ptr<SimpleJournal> journal2 = std::make_shared<SimpleJournal>();
        journal2->append(JournalMethod::ADDED, JournalTarget::FILE, "/tmp/journal.xml", FileData("/tmp/journal.xml"));
        journal2->commitState(CommitTimeType::clock::now());


        REQUIRE_FALSE(journal.merge(journal2));

    }

    //cleanup

    fs::remove("/tmp/journal.xml");


}