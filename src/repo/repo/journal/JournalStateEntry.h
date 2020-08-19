//
// Created by Krzysztof Tulidowicz on 09.03.2020.
//

#ifndef BSC_JOURNALSTATEENTRY_H
#define BSC_JOURNALSTATEENTRY_H

#include "JournalMetaData.h"
#include "JournalMethod.h"
#include "JournalTarget.h"
#include "JournalTypes.h"
#include "SpecialKind.h"
#include <chrono>
#include <core/utils/cereal_include.h>
#include <filesystem>
#include <p2p/modules/filesystem/data/FileData.h>
#include <repo/repository/transformer/PathTransformerRuleSelector.h>

namespace bsc {
    namespace fs = std::filesystem;

    template<JournalTarget target>
    class JournalStateEntry {
    private:
        JournalMethod method = JournalMethod::none;
        std::string destination{};
        std::string source{};// only used in certain cases, like move, rename or for specific parameters
        uint64_t size         = 0;
        fs::perms permissions = fs::perms::none;
        //@todo C++20 this should probably be changed to utc_clock. epoch of file_time_type is unspecified and it may
        //cause problems.
        fs::file_time_type modificationTime{};
        ChecksumType resourceChecksum{};// checksum of the file.
        ChecksumType checksum{};        // checksum of the whole structure
    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(CEREAL_NVP(method),
               CEREAL_NVP(target),
               CEREAL_NVP(destination),
               CEREAL_NVP(source),
               CEREAL_NVP(size),
               CEREAL_NVP(resourceChecksum),
               CEREAL_NVP(modificationTime),
               CEREAL_NVP(permissions));
        }

        friend class cereal::access;

    public:
        JournalStateEntry(JournalMethod method, const PathType& path, const bsc::FileData& data)
            : method(method), destination(path) {
            size             = data.getSize();
            modificationTime = data.getModificationTime();
            permissions      = data.getPermissions();
            resourceChecksum = data.getSha256hash();
        };

        JournalStateEntry() = default;

        JournalMethod getMethod() const { return method; }

        fs::file_time_type getModificationTime() const { return modificationTime; }

        constexpr JournalTarget getTarget() const { return target; }
        const std::string& getDestination() const { return destination; }
        const std::string& getSource() const { return source; }
        fs::perms getPermissions() const { return permissions; }
        const ChecksumType& getChecksum() const { return checksum; }

        const ChecksumType& getResourceChecksum() const { return resourceChecksum; }

        [[nodiscard]] ChecksumType calculateChecksum() const {
            std::stringstream ss;
            ss << std::to_string(method) << std::to_string(static_cast<std::underlying_type_t<JournalTarget>>(target))
               << destination << source << std::to_string(size)
               << std::to_string(static_cast<std::underlying_type_t<fs::perms>>(permissions))
               << std::to_string(modificationTime.time_since_epoch().count()) << resourceChecksum;

            return bsc::calculateSha1OfString(ss.str());
        }

        uintmax_t getSize() const { return size; }

        bool operator==(const JournalStateEntry<target>& other) const {

            return other.getResourceChecksum() == this->getResourceChecksum() && other.getSize() == this->getSize() &&
                   other.getMethod() == this->getMethod() && other.getDestination() == this->getDestination();
        }
    };

    template<JournalTarget target, JournalTarget otherTarget>
    bool operator==(const JournalStateEntry<target>& first, const JournalStateEntry<otherTarget>& second) {
        return false;
    }

    //    @todo implement
    template<>
    class JournalStateEntry<JournalTarget::transformer> {
    private:
        JournalMethod method = JournalMethod::none;
        PathTransformerRuleSelector transformerRuleSelector;

    public:
        JournalMethod getMethod() const { return method; }

        PathTransformerRuleSelector getTransformerRuleSelector() const { return transformerRuleSelector; }

        [[nodiscard]] ChecksumType calculateChecksum() const {
            std::stringstream ss;
            ss << std::to_string(method);

            return bsc::calculateSha1OfString(ss.str());
        }

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(CEREAL_NVP(method), CEREAL_NVP(transformerRuleSelector));
        }

    public:
        JournalStateEntry() = default;
        JournalStateEntry(JournalMethod method, PathTransformerRuleSelector transformerRuleSelector)
            : method(method), transformerRuleSelector(transformerRuleSelector) {}

    private:
        friend class cereal::access;
    };

}// namespace bsc

#endif