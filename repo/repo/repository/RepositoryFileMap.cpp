//
// Created by stilgar on 25.02.2020.
//

#include "RepositoryFileMap.h"

#include <utility>
namespace bsc {
    const RepositoryFileMap& RepositoryFileMapRenderer::renderMap(JournalPtr journal) {
        //@todo should I check if journal is null?
        auto journalChecksum = journal->getChecksum();
        if (fileMapCache.contains(journalChecksum)) {
            return fileMapCache[journalChecksum];
        }
        //    LOGGER("prepare map jch:" + journal->getChecksum() + " mck " + mapChecksum)
        auto& fileMap = fileMapCache[journalChecksum];
        LOGGER("checksum different, recreate file map")
        //@todo this funcmap should be const and generated in constructor or even static, no need to make it every call to render
        JournalFuncMap funcMap;
        funcMap.setFunc<JournalMethod::add,JournalTarget::file>([&](auto& i) {
            fileMap.attributesMap[pathTransformer->transformFromJournalFormat(i.getDestination())] = RepositoryAttributes(i);
            LOGGER(IStorage::getResourceId(i.getResourceChecksum(), i.getSize()) + " ::: " + i.getDestination());
        });

        funcMap.setFunc<JournalMethod::modify,JournalTarget::file>([&](auto& i) {
            fileMap.attributesMap[pathTransformer->transformFromJournalFormat(i.getDestination())] = RepositoryAttributes(i);
            LOGGER(IStorage::getResourceId(i.getResourceChecksum(), i.getSize()) + " ::: " + i.getDestination());
        });

        //@todo moved file should have two parameters - from to. or, just remove moved and use deleted/added
        funcMap.setFunc<JournalMethod::move,JournalTarget::file>([&](auto& i) {
            fileMap.attributesMap[pathTransformer->transformFromJournalFormat(i.getDestination())] = RepositoryAttributes(i);
        });

        funcMap.setFunc<JournalMethod::remove,JournalTarget::file>([&](auto& i) {
            auto path = pathTransformer->transformFromJournalFormat(i.getDestination());
            fileMap.attributesMap[path] = std::nullopt;
            fileMap.deleteMap[path].setDeleted(true);
            fileMap.deleteMap[path].setDeletionTime(i.getModificationTime());
            //            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getDestination());
        });

        funcMap.setFunc<JournalMethod::forget,JournalTarget::file>([&](auto& i) {
            auto path = pathTransformer->transformFromJournalFormat(i.getDestination());
            fileMap.attributesMap[path] = std::nullopt;
            fileMap.deleteMap[path].setDeleted(false);
            fileMap.deleteMap[path].setDeletionTime(fs::file_time_type::min());
            //            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getDestination());
        });

        funcMap.setFunc<JournalMethod::add,JournalTarget::directory>([&](auto& i) {
            fileMap.attributesMap[pathTransformer->transformFromJournalFormat(i.getDestination())] = RepositoryAttributes(i);
            LOGGER(IStorage::getResourceId(i.getResourceChecksum(), i.getSize()) + " ::: " + i.getDestination());
        });

        funcMap.setFunc<JournalMethod::modify,JournalTarget::directory>([&](auto& i) {
            fileMap.attributesMap[pathTransformer->transformFromJournalFormat(i.getDestination())] = RepositoryAttributes(i);
            LOGGER(IStorage::getResourceId(i.getResourceChecksum(), i.getSize()) + " ::: " + i.getDestination());
        });

        //@todo order of operations is important. file moved and recreated with the same path may be overwritten and then moved if operations are in wrong order. that need to be preserved when I implement moves.
        //@todo moved file should have two parameters - from to. or, just remove moved and use deleted/added
        funcMap.setFunc<JournalMethod::move,JournalTarget::directory>([&](auto& i) {
            fileMap.attributesMap[pathTransformer->transformFromJournalFormat(i.getDestination())] = RepositoryAttributes(i);
        });

        funcMap.setFunc<JournalMethod::remove,JournalTarget::directory>([&](auto& i) {
            auto path = pathTransformer->transformFromJournalFormat(i.getDestination());
            fileMap.attributesMap[path] = std::nullopt;
            fileMap.deleteMap[path].setDeleted(true);
            fileMap.deleteMap[path].setDeletionTime(i.getModificationTime());
            //            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getDestination());
        });

        funcMap.setFunc<JournalMethod::forget,JournalTarget::directory>([&](auto& i) {
            auto path = pathTransformer->transformFromJournalFormat(i.getDestination());
            fileMap.attributesMap[path] = std::nullopt;
            fileMap.deleteMap[path].setDeleted(false);
            fileMap.deleteMap[path].setDeletionTime(fs::file_time_type::min());
            //            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getDestination());
        });

        funcMap.setFunc<JournalMethod::add,JournalTarget::feature>([&](auto& i) {
          auto path = pathTransformer->transformFromJournalFormat(i.getDestination());
            //@todo implement
        });

        funcMap.setFunc<JournalMethod::remove,JournalTarget::feature>([&](auto& i) {
          auto path = pathTransformer->transformFromJournalFormat(i.getDestination());
            //@todo implement
        });

        funcMap.setFunc<JournalMethod::modify,JournalTarget::feature>([&](auto& i) {
          auto path = pathTransformer->transformFromJournalFormat(i.getDestination());
            //@todo implement
        });


        //@todo set attributes for ignores.
        journal->replay(funcMap);
        return fileMap;
    }
    RepositoryFileMapRenderer::RepositoryFileMapRenderer(std::shared_ptr<IPathTransformer> pathTransformer) : pathTransformer(std::move(pathTransformer)) {}

    auto RepositoryFileMap::operator[](const fs::path& path) const -> decltype(attributesMap.at(fs::current_path())) {
        if (!attributesMap.contains(path)) {
            return emptyAttribute;
        }
        return attributesMap.at(path);
    }

    auto RepositoryFileMap::begin() const -> decltype(attributesMap.begin()) {
        return attributesMap.begin();
    }

    auto RepositoryFileMap::end() const -> decltype(attributesMap.end()) {
        return attributesMap.end();
    }


    auto RepositoryFileMap::getSize(const fs::path& path) const {
        return attributesMap.at(path)->getSize();
    }

    decltype(RepositoryFileMap::attributesMap) RepositoryFileMap::subMap(const fs::path& root) const {
        decltype(RepositoryFileMap::attributesMap) result;
        for (const auto& [path, value] : attributesMap) {
            if (path.string().find(root.string()) != std::string::npos) {
                result[path] = value;
            }
        }
        return result;
    }

    fs::file_time_type RepositoryFileMap::getDeletionTime(const fs::path& path) const {
        if (!deleteMap.contains(path)) {
            return defaultDeleteInfo.getDeletionTime();
        }
        return deleteMap.at(path).getDeletionTime();
    }

    auto RepositoryFileMap::isDeleted(const fs::path& path) const -> decltype(deleteMap.at(path).isDeleted()) {
        if (!deleteMap.contains(path)) {
            return defaultDeleteInfo.isDeleted();
        }
        return deleteMap.at(path).isDeleted();
    }


    bool RepositoryFileMap::DeleteInfo::isDeleted() const {
        return deleted;
    }

    fs::file_time_type RepositoryFileMap::DeleteInfo::getDeletionTime() const {
        return deletionTime;
    }

    void RepositoryFileMap::DeleteInfo::setDeletionTime(fs::file_time_type deletionTime) {
        DeleteInfo::deletionTime = deletionTime;
    }

    void RepositoryFileMap::DeleteInfo::setDeleted(bool deleted) {
        DeleteInfo::deleted = deleted;
    }
}// namespace bsc