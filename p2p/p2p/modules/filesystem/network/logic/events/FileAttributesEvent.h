//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_FILEATTRIBUTESEVENT_H
#define BASYCO_FILEATTRIBUTESEVENT_H


#include <p2p/logic/IEvent.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NetworkingEvent.h>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

enum class FileAttributesEventId {
    ATTRIBUTES_REQUESTED,
    ATTRIBUTES_RECEIVED,
};


class FileAttributesEvent : public NetworkingEvent<FileAttributesEventId> {

    fs::path path;

public:
    FileAttributesEvent(BasePacket::IdType requestId);

    const std::experimental::filesystem::path &getPath() const;

    void setPath(const std::experimental::filesystem::path &path);


};


#endif //BASYCO_FILEATTRIBUTESEVENT_H
