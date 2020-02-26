//
// Created by stilgar on 25.02.2020.
//

#ifndef BSC_REPOSITORYFILEMAP_H
#define BSC_REPOSITORYFILEMAP_H
#include "RepositoryAttributes.h"
#include <filesystem>
#include <repo/journal/IJournal.h>
#include <repo/repository/transformer/IPathTransformer.h>


namespace bsc {
    namespace fs = std::filesystem;
    //@todo Store more data in RepositoryFileMap - it should know modification date of each path and type (dir/file)
    class RepositoryFileMap {
    public:
        using ValueType = std::optional<RepositoryAttributes>;
        class DeleteInfo {
        private:
            bool deleted = false;
            //@todo store utc_clock value instead of old time_t
            fs::file_time_type deletionTime = fs::file_time_type::min();

        public:
            bool isDeleted() const;

            void setDeleted(bool deleted);

            fs::file_time_type getDeletionTime() const;

            void setDeletionTime(fs::file_time_type deletionTime);
        };

    private:
        const ValueType emptyAttribute = std::nullopt;
        std::map<fs::path, ValueType> attributesMap;
        const DeleteInfo defaultDeleteInfo{};
        std::map<fs::path, DeleteInfo> deleteMap;
        RepositoryFileMap(RepositoryFileMap&) = delete;
        RepositoryFileMap(RepositoryFileMap&&) = delete;

    public:
        RepositoryFileMap() = default;
        auto operator[](const fs::path& path) const -> decltype(attributesMap.at(fs::current_path()));

        auto contains(const fs::path& path) const {
            return attributesMap.contains(path);
        }
        auto getSize(const fs::path& path) const;
        decltype(attributesMap) subMap(const fs::path& root) const;
        auto begin() const -> decltype(attributesMap.begin());
        auto end() const -> decltype(attributesMap.end());
        fs::file_time_type getDeletionTime(const fs::path& path) const;
        auto isDeleted(const fs::path& path) const -> decltype(deleteMap.at(path).isDeleted());
        friend class RepositoryFileMapRenderer;
    };

    class RepositoryFileMapRenderer {
        std::shared_ptr<IPathTransformer> pathTransformer;
        std::map<ChecksumId, RepositoryFileMap> fileMapCache{};

    public:
        RepositoryFileMapRenderer(std::shared_ptr<IPathTransformer> pathTransformer);

    public:
        const RepositoryFileMap& renderMap(JournalPtr journal);
    };
}// namespace bsc

#endif