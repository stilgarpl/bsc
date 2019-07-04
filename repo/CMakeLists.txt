#cmake_minimum_required(VERSION 3.7)
#project(basyco)

#set(CMAKE_CXX_STANDARD 17)




find_package(Crypto++ REQUIRED)



set(SOURCE_FILES repo/journal/SimpleJournal.cpp repo/journal/SimpleJournal.h repo/repository/Repository.cpp repo/repository/Repository.h repo/journal/IJournal.h repo/journal/PartialJournal.cpp repo/journal/PartialJournal.h repo/journal/JournalState.cpp repo/journal/JournalState.h repo/journal/JournalMethod.h repo/journal/JournalTypes.h repo/journal/network/packet/JournalGroup.cpp repo/journal/network/packet/JournalGroup.h repo/journal/network/logic/sources/JournalSource.cpp repo/journal/network/logic/sources/JournalSource.h repo/journal/network/logic/events/JournalRequestEvent.cpp repo/journal/network/logic/events/JournalRequestEvent.h repo/context/RepositoryContext.cpp repo/context/RepositoryContext.h repo/repository/RepositoryManager.cpp repo/repository/RepositoryManager.h repo/node/RepoModule.cpp repo/node/RepoModule.h repo/journal/network/logic/events/JournalResponseEvent.cpp repo/journal/network/logic/events/JournalResponseEvent.h repo/journal/network/logic/actions/JournalActions.cpp repo/journal/network/logic/actions/JournalActions.h repo/repository/storage/IStorage.cpp repo/repository/storage/IStorage.h repo/repository/storage/InternalStorage.cpp repo/repository/storage/InternalStorage.h repo/repository/IRepository.h repo/repository/storage/network/logic/sources/StorageSource.cpp repo/repository/storage/network/logic/sources/StorageSource.h repo/repository/storage/network/logic/events/StorageResourceRequestEvent.cpp repo/repository/storage/network/logic/events/StorageResourceRequestEvent.h repo/repository/storage/network/packet/StorageQuery.cpp repo/repository/storage/network/packet/StorageQuery.h repo/repository/storage/network/logic/actions/StorageActions.cpp repo/repository/storage/network/logic/actions/StorageActions.h repo/repository/storage/network/packet/StorageManifest.cpp repo/repository/storage/network/packet/StorageManifest.h repo/repository/storage/StorageManifest.cpp repo/repository/storage/StorageManifest.h repo/repository/storage/StorageResourceIdentificator.cpp repo/repository/storage/StorageResourceIdentificator.h repo/repository/transformer/PathTransformer.cpp repo/repository/transformer/PathTransformer.h repo/repository/transformer/IPathTransformer.h repo/repository/transformer/ITransformRule.h repo/repository/transformer/rules/HomeDirRule.cpp repo/repository/transformer/rules/HomeDirRule.h repo/repository/transformer/rules/TmpRule.cpp repo/repository/transformer/rules/TmpRule.h repo/repository/transformer/rules/CustomRule.cpp repo/repository/transformer/rules/CustomRule.h repo/journal/JournalTarget.cpp repo/journal/JournalTarget.h repo/repository/logic/actions/RepositoryActions.cpp repo/repository/logic/actions/RepositoryActions.h repo/repository/network/RepoQuery.cpp repo/repository/network/RepoQuery.h repo/repository/logic/evaluators/RepoEvaluators.cpp repo/repository/logic/evaluators/RepoEvaluators.h repo/repository/network/RepoProcessors.cpp repo/repository/network/RepoProcessors.h repo/repository/strategy/Strategy.h)
add_library(repo SHARED ${SOURCE_FILES})
target_include_directories(repo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} SYSTEM ${CRYPTO++_INCLUDE_DIR})
message(CURRENT SOURCE: ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(repo PUBLIC p2p_core module_command module_filesystem ${CRYPTO++_LIBRARIES} ${LIBS} stdc++fs)
install(TARGETS repo COMPONENT repo)

add_subdirectory(test)
