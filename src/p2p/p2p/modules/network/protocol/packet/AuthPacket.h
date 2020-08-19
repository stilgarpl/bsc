//
// Created by Krzysztof Tulidowicz on 29.08.17.
//

#ifndef BSC_AUTHPACKET_H
#define BSC_AUTHPACKET_H


namespace bsc {
    class AuthPacket : public BasePacket {
    private:
        //@todo zmienic to na jakis sensowny sposob autentykacji
        std::string login;
        std::string password;

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar & cereal::base_class<BasePacket>(this);
        }


        friend class cereal::access;

    };
}

CEREAL_REGISTER_TYPE(bsc::AuthPacket);

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::AuthPacket
)
bsc::;

#endif //BSC_AUTHPACKET_H
