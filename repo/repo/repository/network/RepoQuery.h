//
// Created by stilgar on 05.02.19.
//

#ifndef BASYCO_REPOQUERY_H
#define BASYCO_REPOQUERY_H


#include <core/utils/cereal_include.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/info/PacketInfo.h>
#include <repo/repository/IRepository.h>
#include <repo/journal/IJournal.h>

struct RepoQuery : public PacketGroup {

public:


    class Request : public Packet<RepoQuery, RepoQuery::Request> {
    private:
        IRepository::RepoIdType repoId;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<RepoQuery, RepoQuery::Request>>(this));
            ar(repoId);
        }

    public:
        const IRepository::RepoIdType &getRepoId() const;

        void setRepoId(const IRepository::RepoIdType &repoId);

    private:

        friend class cereal::access;

    public:
        void process(Context::Ptr context) override;
    };

    class Response : public Packet<RepoQuery, RepoQuery::Response> {
        IRepository::RepoIdType repoId;
        bool exists;
        //@todo maybe just some journal descriptor instead of full journal?
        JournalPtr journal;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<RepoQuery, RepoQuery::Response>>(this));
            ar(repoId, exists, journal);
        }

    public:
        const IRepository::RepoIdType &getRepoId() const {
            return repoId;
        }

        void setRepoId(const IRepository::RepoIdType &repoId) {
            Response::repoId = repoId;
        }

        bool isExists() const {
            return exists;
        }

        void setExists(bool exists) {
            Response::exists = exists;
        }

        const JournalPtr &getJournal() const {
            return journal;
        }

        void setJournal(const JournalPtr &journal) {
            Response::journal = journal;
        }

        void process(Context::Ptr context) override {
            BasePacket::process(context);
            LOGGER(" REPO Q RESPONSE RECEIVED")
        }

    private:

        friend class cereal::access;

    };
};


CEREAL_REGISTER_TYPE(RepoQuery::Request)
CEREAL_REGISTER_TYPE(RepoQuery::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, RepoQuery::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, RepoQuery::Response)

#endif //BASYCO_REPOQUERY_H
