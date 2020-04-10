//
// Created by stilgar on 09.04.2020.
//
#include <catch2/catch.hpp>
#include <core/file/FileType.h>
#include <core/file/FileTypeDecoder.h>
#include "../../../tester/tester/Tester.h"

using namespace bsc;

TEST_CASE("Filetype test") {

    SECTION("application/json") {
        FileType f = FileType::make("application/json");
        REQUIRE(f.type == "json");
        REQUIRE(f.typeGroup == "application");
    }

    SECTION("image/jpeg") {
        FileType f = FileType::make("image/jpeg");
        REQUIRE(f.type == "jpeg");
        REQUIRE(f.typeGroup == "image");
    }

    SECTION("invalid mime string") {
        REQUIRE_THROWS_MATCHES(FileType::make("invalid"),
                               FileTypeParseException,
                               Catch::Matchers::Message("Mime string [invalid] does not match (\\w+)/(\\w+) pattern"));
    }
}

TEST_CASE("File type decoder test") {
    Tester::TestDirWithResources testDirWithResources;
    auto path = testDirWithResources.getTestDirPath();
    const std::string gifFilename = "test.gif";
    const std::string pngFilename = "test.png";
    const std::string badPngFilename = "png_with_bad_extension.txt";
    const std::string txtFilename = "test.txt";
    FileTypeDecoder decoder;
    SECTION("txt") {
        auto fileType = decoder.getTypeForFile(path / txtFilename);
        REQUIRE(fileType.typeGroup == "text");
        REQUIRE(fileType.type == "plain");
    }SECTION("gif") {
        auto fileType = decoder.getTypeForFile(path / gifFilename);
        REQUIRE(fileType.typeGroup == "image");
        REQUIRE(fileType.type == "gif");
    }SECTION("png") {
        auto fileType = decoder.getTypeForFile(path / pngFilename);
        REQUIRE(fileType.typeGroup == "image");
        REQUIRE(fileType.type == "png");
    }SECTION("bad png") {
        auto fileType = decoder.getTypeForFile(path / badPngFilename);
        REQUIRE(fileType.typeGroup == "image");
        REQUIRE(fileType.type == "png");
    }

}