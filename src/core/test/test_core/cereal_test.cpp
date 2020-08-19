//
// Created by Krzysztof Tulidowicz on 20.02.2020.
//
#include <core/utils/cereal_include.h>
#include <catch2/catch.hpp>

TEST_CASE("Custom path serializer for cereal") {


    std::stringstream dataStorage;
    cereal::BinaryOutputArchive oa(dataStorage);
    cereal::BinaryInputArchive ia(dataStorage);

    //given
    std::filesystem::path path = "/home/";

    oa << path;

    std::filesystem::path newPath;
    ia >> newPath;

    REQUIRE(path == newPath);


}