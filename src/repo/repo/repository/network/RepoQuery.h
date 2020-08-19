//
// Created by Krzysztof Tulidowicz on 05.02.19.
//

#ifndef BSC_REPOQUERY_H
#define BSC_REPOQUERY_H


#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <repo/journal/Journal.h>
#include <repo/repository/IRepository.h>

namespace bsc {

    struct RepoQuery : public bsc::PacketGroup {

    public:


        class Request : public bsc::Packet<RepoQuery, RepoQuery::Request> {
        private:
            IRepository::RepoIdType repoId;

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<RepoQuery, RepoQuery::Request>>
                           (this));
                ar(repoId);
            }

        public:
            const IRepository::RepoIdType& getRepoId() const;

            void setRepoId(const IRepository::RepoIdType& repoId);

        private:

            friend class cereal::access;

        public:
        };

        class Response : public bsc::Packet<RepoQuery, RepoQuery::Response> {
            IRepository::RepoIdType repoId;
            bool exists;
            //@todo maybe just some journal descriptor instead of full journal?
            JournalPtr journal;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<RepoQuery, RepoQuery::Response>>
                           (this));
                ar(repoId, exists, journal);
            }

        public:
            const IRepository::RepoIdType& getRepoId() const {
                return repoId;
            }

            void setRepoId(const IRepository::RepoIdType& repoId) {
                Response::repoId = repoId;
            }

            bool isExists() const {
                return exists;
            }

            void setExists(bool exists) {
                Response::exists = exists;
            }

            const JournalPtr& getJournal() const {
                return journal;
            }

            void setJournal(const JournalPtr& journal) {
                Response::journal = journal;
            }


        private:

            friend class cereal::access;

        };
    };

}
CEREAL_REGISTER_TYPE(bsc::RepoQuery::Request)
CEREAL_REGISTER_TYPE(bsc::RepoQuery::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::RepoQuery::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::RepoQuery::Response)

#endif //BSC_REPOQUERY_H
