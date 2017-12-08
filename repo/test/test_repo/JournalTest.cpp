//
// Created by stilgar on 07.12.17.
//
#include <catch/catch.hpp>
#include <repo/journal/SimpleJournal.h>


TEST_CASE("Journal merge test") {
    SimpleJournal journal;
    journal.append(JournalMethod::ADDED, "/tmp/dupa.txt");
    journal.append(JournalMethod::DELETED, "/tmp/to_remove.txt");
    journal.commitState();
    journal.append(JournalMethod::UPDATED, "/tmp/dupa.txt");
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


        journal1->append(JournalMethod::ADDED, "/bin/zsh");
        journal1->commitState();


        REQUIRE(journal.merge(journal1));

    }

    SECTION("fail merge") {

        std::shared_ptr<SimpleJournal> journal2 = std::make_shared<SimpleJournal>();
        journal2->append(JournalMethod::ADDED, "/tmp/journal.xml");
        journal2->commitState();


        REQUIRE_FALSE(journal.merge(journal2));

    }


}