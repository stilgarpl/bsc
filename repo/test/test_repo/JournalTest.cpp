//
// Created by stilgar on 07.12.17.
//
#include <catch2/catch.hpp>
#include <repo/journal/SimpleJournal.h>

TEST_CASE("Journal deterministic hash test") {
    //@todo implement - the problem is that clock is included in the checksum and it's always changing.
    SimpleJournal journal;
    journal.append(JournalMethod::ADDED, JournalTarget::FILE, "/tmp/dupa.txt", FileData("/tmp/dupa.txt"));
    journal.commitState();
    //@todo find a way to enable this:
//    REQUIRE_THAT(journal.getChecksum(), Catch::Matchers::Equals("80E58CD2BD2038194EDA8379D3A58319DB94F704"));
}


TEST_CASE("Journal merge test") {
    SimpleJournal journal;
    journal.append(JournalMethod::ADDED, JournalTarget::FILE, "/tmp/dupa.txt", FileData("/tmp/dupa.txt"));
    journal.append(JournalMethod::DELETED, JournalTarget::FILE, "/tmp/to_remove.txt", FileData("/tmp/to_remove.txt"));
    journal.commitState();
    journal.append(JournalMethod::MODIFIED, JournalTarget::FILE, "/tmp/dupa.txt", FileData("/tmp/to_remove.txt"));
    journal.replay();
    journal.commitState();
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
        journal1->commitState();


        REQUIRE(journal.merge(journal1));

    }

    SECTION("fail merge") {

        std::shared_ptr<SimpleJournal> journal2 = std::make_shared<SimpleJournal>();
        journal2->append(JournalMethod::ADDED, JournalTarget::FILE, "/tmp/journal.xml", FileData("/tmp/journal.xml"));
        journal2->commitState();


        REQUIRE_FALSE(journal.merge(journal2));

    }


}