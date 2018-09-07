//
// Created by stilgar on 22.11.17.
//

#ifndef BASYCO_SENDATTRIBUTES_H
#define BASYCO_SENDATTRIBUTES_H

#include <p2p/utils/cereal_include.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/info/PacketInfo.h>


#include <filesystem>

namespace fs = std::filesystem;


class SendAttributes : public PacketGroup {

public:
    class Request : public Packet<SendAttributes, SendAttributes::Request> {
    private:
        //fs::path path;
        std::string path;
    public:
        const std::string &getPath() const;

        void setPath(const std::string &path);

    public:
        void process(Context::Ptr context) override;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<Packet<SendAttributes, SendAttributes::Request>>(this);
            ar(path);
        }


        friend class cereal::access;
    };

    class Response : public Packet<SendAttributes, SendAttributes::Response> {
    private:
        // fs::path path;
        std::string path;
        size_t size;
    public:
        const std::string &getPath() const;

        void setPath(const std::string &path);

        size_t getSize() const;

        void setSize(size_t size);

    public:
        void process(Context::Ptr context) override;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<Packet<SendAttributes, SendAttributes::Response>>(this);
            ar(path, size);
        }


        friend class cereal::access;
    };

};


CEREAL_REGISTER_TYPE(SendAttributes::Request);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, SendAttributes::Request);
CEREAL_REGISTER_TYPE(SendAttributes::Response);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, SendAttributes::Response);

#endif //BASYCO_SENDATTRIBUTES_H
