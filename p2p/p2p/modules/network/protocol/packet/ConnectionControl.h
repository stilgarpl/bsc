//
// Created by stilgar on 10.12.2019.
//

#ifndef BASYCO_CONNECTIONCONTROL_H
#define BASYCO_CONNECTIONCONTROL_H


#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>

struct ConnectionControl : public PacketGroup {

    enum class Action {
        DISCONNECT,
    };

    enum class Result {
        SUCCESS,
        FAILURE,
    };

    class Request : public Packet<ConnectionControl, ConnectionControl::Request> {
        Action action;

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(cereal::base_class<Packet<ConnectionControl, ConnectionControl::Request>>(this));
            ar(action);
        }

    public:
        [[nodiscard]] Action getAction() const {
            return action;
        }

        void setAction(Action action) {
            Request::action = action;
        }

    private:

        friend class cereal::access;
    };

    class Response : public Packet<ConnectionControl, ConnectionControl::Response> {
        Result result;
    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(cereal::base_class<Packet<ConnectionControl, ConnectionControl::Response>>(this));
            ar(result);
        }

    public:
        [[nodiscard]] Result getResult() const {
            return result;
        }

        void setResult(Result result) {
            Response::result = result;
        }

    private:

        friend class cereal::access;

    };
};


CEREAL_REGISTER_TYPE(ConnectionControl::Request)
CEREAL_REGISTER_TYPE(ConnectionControl::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, ConnectionControl::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, ConnectionControl::Response)


#endif //BASYCO_CONNECTIONCONTROL_H
