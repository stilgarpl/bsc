#cmake_minimum_required(VERSION 3.7)
#project(basyco)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(test)


find_package(Crypto++ REQUIRED)
find_package(Cereal REQUIRED)
FIND_PACKAGE(CPPexperimental REQUIRED)


message(libs:: ${LIBS})

set(INCLUDES ${Cereal_INCLUDE_DIRS} ${CRYPTO++_INCLUDE_DIR})
message(includ:: ${INCLUDES})
include_directories(${INCLUDES})
set(LIBS ${LIBS} ${CPPEXPERIMENTAL_LIBRARIES} ${CRYPTO++_LIBRARIES})

message(libs:: ${LIBS})


set(SOURCE_FILES repo/journal/SimpleJournal.cpp repo/journal/SimpleJournal.h repo/repository/Repository.cpp repo/repository/Repository.h repo/journal/IJournal.h repo/journal/PartialJournal.cpp repo/journal/PartialJournal.h repo/journal/JournalState.cpp repo/journal/JournalState.h repo/journal/JournalMethod.h repo/journal/JournalTypes.h repo/network/packet/JournalGroup.cpp repo/network/packet/JournalGroup.h repo/network/logic/sources/JournalSource.cpp repo/network/logic/sources/JournalSource.h repo/network/logic/events/JournalRequestEvent.cpp repo/network/logic/events/JournalRequestEvent.h repo/context/RepositoryContext.cpp repo/context/RepositoryContext.h repo/repository/RepositoryManager.cpp repo/repository/RepositoryManager.h repo/node/RepoModule.cpp repo/node/RepoModule.h repo/network/logic/events/JournalResponseEvent.cpp repo/network/logic/events/JournalResponseEvent.h repo/network/logic/actions/JournalActions.cpp repo/network/logic/actions/JournalActions.h)
add_library(repo SHARED ${SOURCE_FILES})
target_include_directories(repo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
message(CURRENT SOURCE: ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(repo p2p ${LIBS})

