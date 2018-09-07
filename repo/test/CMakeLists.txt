set(CMAKE_CXX_STANDARD 17)

enable_testing()
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})
#
#find_package(Crypto++ REQUIRED)
#find_package(Cereal REQUIRED)
#FIND_PACKAGE(CPPexperimental REQUIRED)


message(libs:: ${LIBS})

#set(INCLUDES ${Cereal_INCLUDE_DIRS} ${CRYPTO++_INCLUDE_DIR})
#message(includ:: ${INCLUDES})
#include_directories(${INCLUDES})
#set(LIBS ${LIBS} ${CPPEXPERIMENTAL_LIBRARIES} ${CRYPTO++_LIBRARIES} ${GTEST_BOTH_LIBRARIES})

message(libs::test ${LIBS})


set(SOURCE_FILES test_repo/test_main.cpp test_repo/JournalTest.cpp test_repo/repo_test.cpp)
add_executable(test_repo ${SOURCE_FILES})
target_include_directories(test_repo PUBLIC test_repo)
#message(CURRENT SOURCE: ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_repo p2p repo ${LIBS})
