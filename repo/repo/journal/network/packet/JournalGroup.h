//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALGROUP_H
#define BASYCO_JOURNALGROUP_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <repo/journal/SimpleJournal.h>


class JournalGroup : public PacketGroup {
public:
    class Request : public Packet<JournalGroup, Request> {
        std::string repoId; //repository with journal we're requesting

    private:
        template<class Archive>
        void serialize(Archive &ar) {

            ar(cereal::base_class<Packet<JournalGroup, JournalGroup::Request> >(this), repoId);
        }

    public:
        const std::string &getRepoId() const;

        void setRepoId(const std::string &repoId);

    private:
        friend class cereal::access;

    public:
        void process(Context &context) override;
    };

    class Response : public Packet<JournalGroup, Response> {
        std::string repoId;
        JournalPtr journal;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<JournalGroup, JournalGroup::Response> >(this), journal);
        }

    public:
        const std::string &getRepoId() const;

        void setRepoId(const std::string &repoId);

        const JournalPtr getJournal() const;

        void setJournal(const JournalPtr journal);

    private:
        friend class cereal::access;

    public:
        void process(Context &context) override;
    };

};

CEREAL_REGISTER_TYPE(JournalGroup::Request);
CEREAL_REGISTER_TYPE(JournalGroup::Response);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, JournalGroup::Request);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, JournalGroup::Response);


#endif //BASYCO_JOURNALGROUP_H
