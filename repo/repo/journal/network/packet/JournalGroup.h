//
// Created by stilgar on 18.10.17.
//

#ifndef BSC_JOURNALGROUP_H
#define BSC_JOURNALGROUP_H


#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <repo/journal/SimpleJournal.h>

namespace bsc {
    class JournalGroup : public bsc::PacketGroup {
    public:
        class Request : public bsc::Packet<JournalGroup, Request> {
            std::string repoId; //repository with journal we're requesting

        private:
            template<class Archive>
            void serialize(Archive& ar) {

                ar(cereal::base_class<Packet<JournalGroup, JournalGroup::Request> >(this), repoId);
            }

        public:
            const std::string& getRepoId() const;

            void setRepoId(const std::string& repoId);

        private:
            friend class cereal::access;


        };

        class Response : public bsc::Packet<JournalGroup, Response> {
            std::string repoId;
            JournalPtr journal;

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<JournalGroup, JournalGroup::Response> >(this), journal);
            }

        public:
            const std::string& getRepoId() const;

            void setRepoId(const std::string& repoId);

            JournalPtr getJournal() const;

            void setJournal(JournalPtr journal);

        private:
            friend class cereal::access;

        };

    };
}
CEREAL_REGISTER_TYPE(bsc::JournalGroup::Request)
CEREAL_REGISTER_TYPE(bsc::JournalGroup::Response)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::JournalGroup::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::JournalGroup::Response)


#endif //BSC_JOURNALGROUP_H
