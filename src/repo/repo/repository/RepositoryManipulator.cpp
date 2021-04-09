//
// Created by Krzysztof Tulidowicz on 26.02.2020.
//

#include "RepositoryManipulator.h"
#include "IRepository.h"


void bsc::RepositoryManipulator::restoreAttributes(const fs::path& path, const bsc::RepositoryFileMap::ValueType& attributes) {

    auto journal = repository.getJournal();                 //@todo if repo.setJournal is removed, then this call can be moved to constructor
    auto& pathTransformer = repository.getPathTransformer();//@todo this one too can be moved to constructor

    if (attributes && fs::exists(path)) {
        fs::permissions(path, attributes->getPermissions());
        fs::last_write_time(path, attributes->getModificationTime());
    }
}
void bsc::RepositoryManipulator::persist(fs::path path, const bsc::RepositoryFileMap::ValueType& attributes) {
    logger.debug("persist: " + path.string());
    auto journal = repository.getJournal();
    auto& pathTransformer = repository.getPathTransformer();
    if (path.is_relative()) {
        path = fs::canonical(fs::current_path() / path);
        logger.debug("after canonical: " + path.string());
    }

    if (attributes) {
        //file exists in map! update mode
        auto target = !attributes->isDirectory() ? JournalTarget::file : JournalTarget::directory;
        //@todo check if file was actually changed.
        journal->append(JournalMethod::modify, target,
                        pathTransformer.transformToJournalFormat(path),
                        bsc::FileData(path));


    } else {
        auto target = !fs::is_directory(path) ? JournalTarget::file : JournalTarget::directory;


        journal->append(JournalMethod::add, target,
                        pathTransformer.transformToJournalFormat(path),
                        bsc::FileData(path));
    }
}
void bsc::RepositoryManipulator::forget(fs::path path, const bsc::RepositoryFileMap::ValueType& attributes) {
    auto journal = repository.getJournal();                 //@todo if repo.setJournal is removed, then this call can be moved to constructor
    auto& pathTransformer = repository.getPathTransformer();//@todo this one too can be moved to constructor
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always setDirect to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    if (attributes) {
        journal->append(JournalMethod::forget,
                        attributes->isDirectory() ? JournalTarget::directory : JournalTarget::file,
                        pathTransformer.transformToJournalFormat(path),
                        attributes->toFileData(path));
    } else {
        //nothing to forget!
    }
}
void bsc::RepositoryManipulator::remove(fs::path path, const bsc::RepositoryFileMap::ValueType& attributes) {
    auto journal = repository.getJournal();                 //@todo if repo.setJournal is removed, then this call can be moved to constructor
    auto& pathTransformer = repository.getPathTransformer();//@todo this one too can be moved to constructor
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always setDirect to the right value
        path = fs::canonical(fs::current_path() / path);
    }

    if (attributes) {
        if (!fs::is_directory(path)) {
            journal->append(JournalMethod::remove, JournalTarget::file,
                            pathTransformer.transformToJournalFormat(path),
                            attributes->toFileData(path));

        } else {
            journal->append(JournalMethod::remove, JournalTarget::directory,
                            pathTransformer.transformToJournalFormat(path),
                            attributes->toFileData(path));
            //@todo delete everything recursively ... or maybe do it in replayCurrentState?
        }
    } else {
        //nothing to forget!
    }
}
void bsc::RepositoryManipulator::ignore(fs::path path, const bsc::RepositoryFileMap::ValueType& attributes) {
    auto journal = repository.getJournal();                 //@todo if repo.setJournal is removed, then this call can be moved to constructor
    auto& pathTransformer = repository.getPathTransformer();//@todo this one too can be moved to constructor
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always setDirect to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    if (!fs::is_directory(path)) {
        journal->append(JournalMethod::ignore, JournalTarget::file,
                        pathTransformer.transformToJournalFormat(path),
                        bsc::FileData(path));

    } else {
        journal->append(JournalMethod::ignore, JournalTarget::directory,
                        pathTransformer.transformToJournalFormat(path),
                        bsc::FileData(path));
    }
}
void bsc::RepositoryManipulator::trash(const fs::path& path) {

    //@todo implement trashing
    fs::remove(path);
}
void bsc::RepositoryManipulator::restoreFileFromStorage(const fs::path& path, const bsc::RepositoryFileMap::ValueType& attributes) {
    if (attributes) {
        if (attributes->isDirectory())  {
            fs::create_directories(path);
        } else {
            repository.getStorage()->restore(attributes->getResourceId(), path);
        }
    }
}
bsc::RepositoryManipulator::RepositoryManipulator(bsc::IRepository& repository) : repository(repository) {}

void bsc::RepositoryManipulator::commit(IStorage& storage) {
    auto journal = repository.getJournal();                 //@todo if repo.setJournal is removed, then this call can be moved to constructor
    auto& pathTransformer = repository.getPathTransformer();//@todo this one too can be moved to constructor
    //@todo move this funcMap outside, so it's not remade every time -- on the other hand, it needs storage reference
    JournalFuncMap funcMap;
    funcMap.setFunc<JournalMethod::add,JournalTarget::file>([&](auto& i) {
        //@todo add error handling - what if file was removed between persisting and storing? if storage fails, commit should fail
        storage.store(bsc::calculateSha1OfFile(pathTransformer.transformFromJournalFormat(i.getDestination())),
                      fs::file_size(pathTransformer.transformFromJournalFormat(i.getDestination())),
                      pathTransformer.transformFromJournalFormat(i.getDestination()));
        logger.debug("commit: added file " + pathTransformer.transformFromJournalFormat(i.getDestination()).string());
    });

    funcMap.setFunc<JournalMethod::modify,JournalTarget::file>([&](auto& i) {
        storage.store(bsc::calculateSha1OfFile(pathTransformer.transformFromJournalFormat(i.getDestination())),
                      fs::file_size(pathTransformer.transformFromJournalFormat(i.getDestination())),
                      pathTransformer.transformFromJournalFormat(i.getDestination()));
        logger.debug("commit: modified file " + pathTransformer.transformFromJournalFormat(i.getDestination()).string());
    });

    journal->replayCurrentState(funcMap);
    journal->commitState(CommitTimeType::clock::now());
}
