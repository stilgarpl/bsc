//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_SENDFILE_H
#define BASYCO_SENDFILE_H

#include "p2p/utils/cereal_include.h"
#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

class SendFile : public PacketGroup {
public:

    class Request : public Packet<SendFile, SendFile::Request> {
    private:
        std::string filePath;
        size_t begin = 0, end = 0;

    public:
        void process(Context::Ptr context) override;

        const std::string &getFilePath() const;

        void setFilePath(const std::string &filePath);

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<Packet<SendFile, SendFile::Request>>(this) &
            filePath & begin & end;


        }

    public:
        size_t getBegin() const;

        void setBegin(size_t begin);

        size_t getEnd() const;

        void setEnd(size_t end);

    private:
        friend class cereal::access;
    };

    class Response : public Packet<SendFile, SendFile::Response> {
    private:
        std::string filePath;
        std::vector<char> data;
        size_t begin = 0, end = 0;
    public:
        void process(Context::Ptr context) override;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<Packet<SendFile, SendFile::Response>>(this)
            & filePath
            & data & begin & end;

        }

        friend class cereal::access;

    public:
        ///@todo really, we should move it to some other class. it's ridiculous to have load and save function in THE PACKET
        void load_file(fs::path path);

        void save_file() const;

    public:
        const std::string &getFilePath() const;

        void setFilePath(const std::string &filePath);

        const std::vector<char> &getData() const;

        void setData(const std::vector<char> &data);

        size_t getBegin() const;

        void setBegin(size_t begin);

        size_t getEnd() const;

        void setEnd(size_t end);
    };

    friend class cereal::access;

};


CEREAL_REGISTER_TYPE(SendFile::Request);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, SendFile::Request);
CEREAL_REGISTER_TYPE(SendFile::Response);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, SendFile::Response);

#endif //BASYCO_SENDFILE_H
