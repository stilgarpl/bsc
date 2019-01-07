#cmake_minimum_required(VERSION 3.7)
#project(basyco)

set(CMAKE_CXX_STANDARD 17)

#FIND_PACKAGE(CPPexperimental)

find_package(Poco REQUIRED COMPONENTS Net Util Foundation  REQUIRED)
message(libs:: ${LIBS})


message(includ:: ${INCLUDES})
set(LIBS ${CPPEXPERIMENTAL_LIBRARIES})

message(libs:: ${LIBS})

set(SOURCE_FILES basyco/main.cpp)
add_executable(basyco ${SOURCE_FILES})
#target_include_directories(basyco PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(basyco PUBLIC p2p)
target_link_libraries(basyco p2p repo ${LIBS})

