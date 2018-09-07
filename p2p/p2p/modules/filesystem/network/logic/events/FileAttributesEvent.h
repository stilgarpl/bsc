//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_FILEATTRIBUTESEVENT_H
#define BASYCO_FILEATTRIBUTESEVENT_H


#include <p2p/logic/IEvent.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NetworkingEvent.h>

#include <filesystem>

namespace fs = std::filesystem;

enum class FileAttributesEventId {
    ATTRIBUTES_REQUESTED,
    ATTRIBUTES_RECEIVED,
};


class FileAttributesEvent : public NetworkingEvent<FileAttributesEventId> {

    fs::path path;

public:
    FileAttributesEvent(BasePacket::IdType requestId);

    const std::filesystem::path &getPath() const;

    void setPath(const std::filesystem::path &path);


};


#endif //BASYCO_FILEATTRIBUTESEVENT_H
