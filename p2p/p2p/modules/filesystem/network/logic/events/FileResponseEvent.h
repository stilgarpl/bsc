//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_FILERESPONSEEVENT_H
#define BASYCO_FILERESPONSEEVENT_H

#include <p2p/logic/IEvent.h>
#include <p2p/modules/filesystem/network/packet/SendFile.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NetworkingEvent.h>

enum class FileResponseId {
    FILE_RECEIVED,
    CHUNK_RECEIVED,
};

///this probably don't have to be a networking event but it could be to be constistent
class FileResponseEvent : public NetworkingEvent<FileResponseId> {
private:
    fs::path filePath;
    size_t begin, end;
    std::vector<char> data;
    //  SendFile::Response::Ptr response;
public:
//    auto getResponse() const {
//        return response;
//    }
//
//    void setResponse(const SendFile::Response::Ptr r) {
//        response = r;
//    }

    FileResponseEvent(BasePacket::IdType requestId) : NetworkingEvent(requestId) {}

    const std::experimental::filesystem::path &getFilePath() const {
        return filePath;
    }

    void setFilePath(const std::experimental::filesystem::path &filePath) {
        FileResponseEvent::filePath = filePath;
    }

    size_t getBegin() const {
        return begin;
    }

    void setBegin(size_t begin) {
        FileResponseEvent::begin = begin;
    }

    size_t getEnd() const {
        return end;
    }

    void setEnd(size_t end) {
        FileResponseEvent::end = end;
    }

    const std::vector<char> &getData() const {
        return data;
    }

    void setData(const std::vector<char> &data) {
        FileResponseEvent::data = data;
    }

    ///@todo move saveFile from there and SendFile::Response to some other class.
    void saveFile() const {
        if (!fs::exists(filePath)) {
            std::fstream f(filePath, std::ios_base::out | std::ofstream::binary);
            f.close(); //file now exists always
        }
        std::ofstream file(filePath, std::ios::in | std::ios::out | std::ofstream::binary);
        if (end == 0) {
            std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(file));
        } else {
            file.seekp(begin);
            std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(file));
        }
    }

};


#endif //BASYCO_FILERESPONSEEVENT_H
