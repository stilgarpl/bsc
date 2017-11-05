//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALGROUP_H
#define BASYCO_JOURNALGROUP_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <repo/journal/Journal.h>
#include <repo/network/logic/sources/JournalSource.h>
#include <p2p/network/protocol/context/LogicContext.h>

class JournalGroup : public PacketGroup {

    class Request : public Packet<JournalGroup, Request> {
        std::string repoId; //repository with journal we're requesting

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(repoId);
        }


        friend class cereal::access;

    public:
        void process(Context &context) override {
            BasePacket::process(context);
            auto lc = context.get<LogicContext>();
            if (lc != nullptr) {
                auto journalSource = lc->getLogicManager().getSource<JournalSource>();

            }
        }
    };

    class Response : public Packet<JournalGroup, Response> {
        Journal journal;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(journal);
        }


        friend class cereal::access;

    public:
        void process(Context &context) override {
            BasePacket::process(context);
        }
    };

};


#endif //BASYCO_JOURNALGROUP_H
