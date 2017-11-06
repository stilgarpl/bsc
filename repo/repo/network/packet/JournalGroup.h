//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALGROUP_H
#define BASYCO_JOURNALGROUP_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <repo/journal/Journal.h>


class JournalGroup : public PacketGroup {
public:
    class Request : public Packet<JournalGroup, Request> {
        std::string repoId; //repository with journal we're requesting

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(repoId);
        }


        friend class cereal::access;

    public:
        void process(Context &context) override;
    };

    class Response : public Packet<JournalGroup, Response> {
        std::string repoId;
        Journal journal;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(journal);
        }

    public:
        const std::string &getRepoId() const;

        void setRepoId(const std::string &repoId);

        const Journal &getJournal() const;

        void setJournal(const Journal &journal);

    private:
        friend class cereal::access;

    public:
        void process(Context &context) override;
    };

};


#endif //BASYCO_JOURNALGROUP_H
