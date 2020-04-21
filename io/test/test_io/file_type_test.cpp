//
// Created by stilgar on 09.04.2020.
//
#include <catch2/catch.hpp>
#include <core/log/Logger.h>
#include <io/file/FileMetaDataReader.h>
#include <io/file/MimeFileType.h>
#include <io/file/MimeFileTypeDecoder.h>
#include <tester/Tester.h>

using namespace bsc;
using namespace std::string_literals;
TEST_CASE("Filetype test") {

    SECTION("application/json") {
        MimeFileType f = MimeFileType::make("application/json");
        REQUIRE(f.type == "json");
        REQUIRE(f.typeGroup == "application");
    }

    SECTION("image/jpeg") {
        MimeFileType f = MimeFileType::make("image/jpeg");
        REQUIRE(f.type == "jpeg");
        REQUIRE(f.typeGroup == "image");
    }

    SECTION("invalid mime string") {
        REQUIRE_THROWS_MATCHES(MimeFileType::make("invalid"),
                               FileTypeParseException,
                               Catch::Matchers::Message("Mime string [invalid] does not match (\\w+)/(\\w+) pattern"));
    }
}

TEST_CASE("File type decoder test") {
    Tester::TestDirWithResources testDirWithResources;
    auto path                        = testDirWithResources.getTestDirPath("mime");
    const std::string gifFilename    = "test.gif";
    const std::string pngFilename    = "test.png";
    const std::string badPngFilename = "png_with_bad_extension.txt";
    const std::string txtFilename    = "test.txt";
    MimeFileTypeDecoder decoder;
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
    const auto& path              = testDirWithResources.getResourcePath("mime");
    const std::string txtFilename = "test.txt";
    const std::string pngFilename = "test.png";
    MimeFileTypeDecoder decoder;
    SECTION("txt") {
        auto fileType = decoder.getTypeForFile(path / txtFilename);
        FileMetaDataReader fileMetaDataReader(fileType);
        const auto& meta = fileMetaDataReader.readMetaData(path / txtFilename);
        //        REQUIRE(meta["wrong_key"].is_null());
        REQUIRE(meta["file"]["size"] == "11");
        REQUIRE(meta["file"]["date"]["year"] == "2020");
        REQUIRE(meta["file"]["date"]["month"] == "4");
        REQUIRE(meta["file"]["date"]["day"].is_string());
        REQUIRE(meta["file"]["date"]["day"] == "10");
        REQUIRE(meta["file"]["time"]["hours"].is_string());
        REQUIRE(meta["file"]["time"]["hours"] == "12");
        REQUIRE(meta["file"]["time"]["minutes"].is_string());
        REQUIRE(meta["file"]["time"]["minutes"] == "1");
        REQUIRE(meta["file"]["time"]["seconds"].is_string());
        REQUIRE(meta["file"]["time"]["seconds"] == "1");
        REQUIRE(meta["date"]["year"].is_string());
        REQUIRE(meta["date"]["year"] == "2020");
        REQUIRE(meta["date"]["month"].is_string());
        REQUIRE(meta["date"]["month"] == "4");
        REQUIRE(meta["date"]["day"].is_string());
        REQUIRE(meta["date"]["day"] == "10");
        REQUIRE(meta["time"]["hours"].is_string());
        REQUIRE(meta["time"]["hours"] == "12");
        REQUIRE(meta["time"]["minutes"].is_string());
        REQUIRE(meta["time"]["minutes"] == "1");
        REQUIRE(meta["time"]["seconds"].is_string());
        REQUIRE(meta["time"]["seconds"] == "1");
    }

    SECTION("png") {
        auto fileType = decoder.getTypeForFile(path / pngFilename);
        FileMetaDataReader fileMetaDataReader(fileType);
        const auto& meta = fileMetaDataReader.readMetaData(path / pngFilename);
        //        REQUIRE(!meta["wrong_key"]);
        LOGGER(meta.dump(2));
        REQUIRE(meta["file"]["size"] == "8495");
        REQUIRE(meta["file"]["date"]["year"].is_string());
        REQUIRE(meta["file"]["date"]["year"] == "2020");
        REQUIRE(meta["file"]["date"]["month"].is_string());
        REQUIRE(meta["file"]["date"]["month"] == "4");
        REQUIRE(meta["file"]["date"]["day"].is_string());
        REQUIRE(meta["file"]["date"]["day"] == "16");
        REQUIRE(meta["file"]["time"]["hours"].is_string());
        REQUIRE(meta["file"]["time"]["hours"] == "10");
        REQUIRE(meta["file"]["time"]["minutes"].is_string());
        REQUIRE(meta["file"]["time"]["minutes"] == "15");
        REQUIRE(meta["file"]["time"]["seconds"].is_string());
        REQUIRE(meta["file"]["time"]["seconds"] == "49");
        REQUIRE(meta["image"]["date"]["year"].is_string());
        REQUIRE(meta["image"]["date"]["year"] == "2020");
        REQUIRE(meta["image"]["date"]["month"].is_string());
        REQUIRE(meta["image"]["date"]["month"] == "04");
        REQUIRE(meta["image"]["date"]["day"].is_string());
        REQUIRE(meta["image"]["date"]["day"] == "16");
        REQUIRE(meta["image"]["time"]["hours"].is_string());
        REQUIRE(meta["image"]["time"]["hours"] == "12");
        REQUIRE(meta["image"]["time"]["minutes"].is_string());
        REQUIRE(meta["image"]["time"]["minutes"] == "15");
        REQUIRE(meta["image"]["time"]["seconds"].is_string());
        REQUIRE(meta["image"]["time"]["seconds"] == "46");
        REQUIRE(meta["date"]["year"].is_string());
        REQUIRE(meta["date"]["year"] == "2020");
        REQUIRE(meta["date"]["month"].is_string());
        REQUIRE(meta["date"]["month"] == "04");
        REQUIRE(meta["date"]["day"].is_string());
        REQUIRE(meta["date"]["day"] == "16");
        REQUIRE(meta["time"]["hours"].is_string());
        REQUIRE(meta["time"]["hours"] == "12");
        REQUIRE(meta["time"]["minutes"].is_string());
        REQUIRE(meta["time"]["minutes"] == "15");
        REQUIRE(meta["time"]["seconds"].is_string());
        REQUIRE(meta["time"]["seconds"] == "46");
    }
}