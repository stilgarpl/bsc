//
// Created by stilgar on 09.04.2020.
//
#include <catch2/catch.hpp>
#include <io/file/FileMetaDataReader.h>
#include <io/file/FileType.h>
#include <io/file/FileTypeDecoder.h>
#include <tester/Tester.h>

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
    auto path                        = testDirWithResources.getTestDirPath();
    const std::string gifFilename    = "test.gif";
    const std::string pngFilename    = "test.png";
    const std::string badPngFilename = "png_with_bad_extension.txt";
    const std::string txtFilename    = "test.txt";
    FileTypeDecoder decoder;
    SECTION("txt") {
        auto fileType = decoder.getTypeForFile(path / txtFilename);
        REQUIRE(fileType.typeGroup == "text");
        REQUIRE(fileType.type == "plain");
    }

    SECTION("gif") {
        auto fileType = decoder.getTypeForFile(path / gifFilename);
        REQUIRE(fileType.typeGroup == "image");
        REQUIRE(fileType.type == "gif");
    }

    SECTION("png") {
        auto fileType = decoder.getTypeForFile(path / pngFilename);
        REQUIRE(fileType.typeGroup == "image");
        REQUIRE(fileType.type == "png");
    }

    SECTION("bad png") {
        auto fileType = decoder.getTypeForFile(path / badPngFilename);
        REQUIRE(fileType.typeGroup == "image");
        REQUIRE(fileType.type == "png");
    }
}

TEST_CASE("FileMetaDataReader test") {
    Tester::Resources testDirWithResources;
    const auto& path              = testDirWithResources.getResourcePath();
    const std::string txtFilename = "test.txt";
    const std::string pngFilename = "test.png";
    FileTypeDecoder decoder;
    SECTION("txt") {
        auto fileType = decoder.getTypeForFile(path / txtFilename);
        FileMetaDataReader fileMetaDataReader(fileType);
        const auto& meta = fileMetaDataReader.readMetaData(path / txtFilename);
        REQUIRE(!meta["wrong_key"].has_value());
        REQUIRE(meta["file.size"].has_value());
        REQUIRE(meta["file.size"].value() == "11");
        REQUIRE(meta["file.date.year"].has_value());
        REQUIRE(meta["file.date.year"].value() == "2020");
        REQUIRE(meta["file.date.month"].has_value());
        REQUIRE(meta["file.date.month"].value() == "4");
        REQUIRE(meta["file.date.day"].has_value());
        REQUIRE(meta["file.date.day"].value() == "10");
        REQUIRE(meta["file.time.hours"].has_value());
        REQUIRE(meta["file.time.hours"].value() == "12");
        REQUIRE(meta["file.time.minutes"].has_value());
        REQUIRE(meta["file.time.minutes"].value() == "1");
        REQUIRE(meta["file.time.seconds"].has_value());
        REQUIRE(meta["file.time.seconds"].value() == "1");
        REQUIRE(meta["date.year"].has_value());
        REQUIRE(meta["date.year"].value() == "2020");
        REQUIRE(meta["date.month"].has_value());
        REQUIRE(meta["date.month"].value() == "4");
        REQUIRE(meta["date.day"].has_value());
        REQUIRE(meta["date.day"].value() == "10");
        REQUIRE(meta["time.hours"].has_value());
        REQUIRE(meta["time.hours"].value() == "12");
        REQUIRE(meta["time.minutes"].has_value());
        REQUIRE(meta["time.minutes"].value() == "1");
        REQUIRE(meta["time.seconds"].has_value());
        REQUIRE(meta["time.seconds"].value() == "1");
    }

    SECTION("png") {
        auto fileType = decoder.getTypeForFile(path / pngFilename);
        FileMetaDataReader fileMetaDataReader(fileType);
        const auto& meta = fileMetaDataReader.readMetaData(path / pngFilename);
        REQUIRE(!meta["wrong_key"].has_value());
        REQUIRE(meta["file.size"].has_value());
        REQUIRE(meta["file.size"].value() == "8495");
        REQUIRE(meta["file.date.year"].has_value());
        REQUIRE(meta["file.date.year"].value() == "2020");
        REQUIRE(meta["file.date.month"].has_value());
        REQUIRE(meta["file.date.month"].value() == "4");
        REQUIRE(meta["file.date.day"].has_value());
        REQUIRE(meta["file.date.day"].value() == "16");
        REQUIRE(meta["file.time.hours"].has_value());
        REQUIRE(meta["file.time.hours"].value() == "10");
        REQUIRE(meta["file.time.minutes"].has_value());
        REQUIRE(meta["file.time.minutes"].value() == "15");
        REQUIRE(meta["file.time.seconds"].has_value());
        REQUIRE(meta["file.time.seconds"].value() == "49");
        REQUIRE(meta["image.date.year"].has_value());
        REQUIRE(meta["image.date.year"].value() == "2020");
        REQUIRE(meta["image.date.month"].has_value());
        REQUIRE(meta["image.date.month"].value() == "04");
        REQUIRE(meta["image.date.day"].has_value());
        REQUIRE(meta["image.date.day"].value() == "16");
        REQUIRE(meta["image.time.hours"].has_value());
        REQUIRE(meta["image.time.hours"].value() == "12");
        REQUIRE(meta["image.time.minutes"].has_value());
        REQUIRE(meta["image.time.minutes"].value() == "15");
        REQUIRE(meta["image.time.seconds"].has_value());
        REQUIRE(meta["image.time.seconds"].value() == "46");
        REQUIRE(meta["date.year"].has_value());
        REQUIRE(meta["date.year"].value() == "2020");
        REQUIRE(meta["date.month"].has_value());
        REQUIRE(meta["date.month"].value() == "04");
        REQUIRE(meta["date.day"].has_value());
        REQUIRE(meta["date.day"].value() == "16");
        REQUIRE(meta["time.hours"].has_value());
        REQUIRE(meta["time.hours"].value() == "12");
        REQUIRE(meta["time.minutes"].has_value());
        REQUIRE(meta["time.minutes"].value() == "15");
        REQUIRE(meta["time.seconds"].has_value());
        REQUIRE(meta["time.seconds"].value() == "46");
    }
}