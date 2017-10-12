//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_SENDFILE_H
#define BASYCO_SENDFILE_H

#include "utils/cereal_include.h"
#include <network/protocol/packet/info/PacketInfo.h>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

class SendFile : public PacketGroup {
public:

    class Request : public Packet<SendFile, SendFile::Request> {
    private:
        std::string filePath;

    public:
        void process(Context &context) override;

        const std::string &getFilePath() const;

        void setFilePath(const std::string &filePath);

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<Packet<SendFile, SendFile::Request>>(this);
            ar & filePath;
        }


        friend class cereal::access;
    };

    class Response : public Packet<SendFile, SendFile::Response> {
    private:
        std::string filePath;
        std::vector<char> data;
    public:
        void process(Context &context) override;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<Packet<SendFile, SendFile::Response>>(this);
            ar & filePath;
            ar & data;

        }

        friend class cereal::access;

    public:
        void load_file(fs::path path);

        void save_file() const;

    public:
        const std::string &getFilePath() const;

        void setFilePath(const std::string &filePath);

        const std::vector<char> &getData() const;

        void setData(const std::vector<char> &data);
    };

    friend class cereal::access;

};

class Dupa {
public:
    class Dupa1 {
    public:
        template<class Archive>
        void serialize(Archive &ar) {
        }


        friend class cereal::access;

        virtual void a() {};
    };

    friend class cereal::access;

};

CEREAL_REGISTER_TYPE(Dupa::Dupa1);

CEREAL_REGISTER_TYPE(SendFile::Request);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, SendFile::Request);
CEREAL_REGISTER_TYPE(SendFile::Response);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, SendFile::Response);

#endif //BASYCO_SENDFILE_H
