//
// Created by stilgar on 09.03.2020.
//

#ifndef BSC_JOURNALSTATEDATA_H
#define BSC_JOURNALSTATEDATA_H

#include "JournalMetaData.h"
#include "JournalMethod.h"
#include "JournalTarget.h"
#include "JournalTypes.h"
#include <cereal/access.hpp>
#include <chrono>
#include <core/utils/cereal_include.h>
#include <filesystem>
#include <p2p/modules/filesystem/data/FileData.h>

namespace bsc {
    namespace fs = std::filesystem;


    template<JournalTarget target>
    class JournalStateData {
    private:
        JournalMethod method = JournalMethod::none;
        std::string destination{};
        std::string source{};//only used in certain cases, like move, rename or for specific parameters
        uint64_t size = 0;
        fs::perms permissions = fs::perms::none;
        //@todo C++20 this should probably be changed to utc_clock. epoch of file_time_type is unspecified and it may cause problems.
        fs::file_time_type modificationTime{};
        ChecksumType resourceChecksum{};//checksum of the file.
        ChecksumType checksum{};        // checksum of the whole structure
    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(CEREAL_NVP(method), CEREAL_NVP(target), CEREAL_NVP(destination), CEREAL_NVP(source), CEREAL_NVP(size), CEREAL_NVP(resourceChecksum),
               CEREAL_NVP(modificationTime),
               CEREAL_NVP(permissions));
        }


        friend class cereal::access;

    public:
        JournalStateData(JournalMethod method, PathType path, bsc::FileData fileData) : method(method), destination(std::move(path)) {
            update(std::move(fileData));
        };

        JournalStateData() = default;

        JournalMethod getMethod() const {
            return method;
        }


        fs::file_time_type getModificationTime() const {
            return modificationTime;
        }


        constexpr JournalTarget getTarget() const {
            return target;
        }
        const std::string& getDestination() const {
            return destination;
        }
        const std::string& getSource() const {
            return source;
        }
        fs::perms getPermissions() const {
            return permissions;
        }
        const ChecksumType& getChecksum() const {
            return checksum;
        }

        void update(bsc::FileData data) {

            // if (!data.isIsDirectory()) {
            size = data.getSize();
            modificationTime = data.getModificationTime();
            permissions = data.getPermissions();
            resourceChecksum = data.getSha256hash();
            //@todo think if data.isDirectory, target = directory?
            //}
        }

        const ChecksumType& getResourceChecksum() const {
            return resourceChecksum;
        }

        const ChecksumType calculateChecksum() const {
            std::string hash = "";
            std::stringstream ss;
            ss << std::to_string(method) << std::to_string(static_cast<std::underlying_type_t<JournalTarget>>(target))
               << destination << source << std::to_string(size) << std::to_string(static_cast<std::underlying_type_t<fs::perms>>(permissions)) << std::to_string(modificationTime.time_since_epoch().count()) << resourceChecksum;

            hash = bsc::calculateSha1OfString(ss.str());

            return hash;
        }

        uintmax_t getSize() const {
            return size;
        }
    };

    //@todo implement
//    template<>
//    class JournalStateData<JournalTarget::feature> {
//    private:
//        JournalMethod method = JournalMethod::none;
//    };
}// namespace bsc

#endif