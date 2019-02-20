//
// Created by stilgar on 05.02.19.
//

#ifndef BASYCO_REPOQUERY_H
#define BASYCO_REPOQUERY_H


#include <p2p/utils/cereal_include.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/info/PacketInfo.h>
#include <repo/repository/IRepository.h>

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


        friend class cereal::access;

    public:
        void process(Context::Ptr context) override;
    };

    class Response : public Packet<RepoQuery, RepoQuery::Response> {
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<RepoQuery, RepoQuery::Response>>(this));
        }


        friend class cereal::access;

    };
};


CEREAL_REGISTER_TYPE(RepoQuery::Request)
CEREAL_REGISTER_TYPE(RepoQuery::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, RepoQuery::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, RepoQuery::Response)

#endif //BASYCO_REPOQUERY_H
