//
// Created by Krzysztof Tulidowicz on 09.04.2020.
//
#include <catch2/catch.hpp>
#include <io/file/FileInfoDecoder.h>
#include <io/file/FileMetaDataReader.h>
#include <io/file/MimeFileType.h>
#include <io/file/MimeFileTypeDecoder.h>
#include <log/log/Logger.h>
#include <testaid/testaid.h>

using namespace bsc;
using namespace std::string_literals;
TEST_CASE("Filetype test") {
    MimeFileTypeFactory factory;

    SECTION("application/json") {
        MimeFileType f = factory.create("application/json");
        REQUIRE(f.type == "json");
        REQUIRE(f.typeGroup == "application");
    }

    SECTION("image/jpeg") {
        MimeFileType f = factory.create("image/jpeg");
        REQUIRE(f.type == "jpeg");
        REQUIRE(f.typeGroup == "image");
    }

    SECTION("image/") {
        MimeFileType f = factory.create("image/");
        REQUIRE(f.type.empty());
        REQUIRE(f.typeGroup == "image");
    }

    SECTION("image") {
        MimeFileType f = factory.create("image");
        REQUIRE(f.type.empty());
        REQUIRE(f.typeGroup == "image");
    }

    SECTION("invalid mime string") {
        REQUIRE_THROWS_MATCHES(factory.create("  invalid"),
                               FileTypeParseException,
                               Catch::Matchers::Message("Mime string [  invalid] does not match (\\w+)/(\\w+) pattern"));
    }
}

TEST_CASE("File type decoder test") {
    testaid::TestDirWithResources testDirWithResources;
    auto path                        = testDirWithResources.getResourcePath("mime");
    const std::string gifFilename    = "test.gif";
    const std::string pngFilename    = "test.png";
    const std::string badPngFilename = "png_with_wrong_extension.txt";
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

TEST_CASE("File info decoder test") {
    testaid::TestDirWithResources testDirWithResources;
    auto path                        = testDirWithResources.getResourcePath("mime");
    const std::string gifFilename    = "test.gif";
    const std::string pngFilename    = "test.png";
    const std::string badPngFilename = "png_with_wrong_extension.txt";
    const std::string txtFilename    = "test.txt";
    FileInfoDecoder decoder;
    SECTION("txt") {
        auto fileInfo = decoder.decodeFileInfo(path / txtFilename);
        REQUIRE(fileInfo.mimeFileType.typeGroup == "text");
        REQUIRE(fileInfo.mimeFileType.type == "plain");
        REQUIRE(fileInfo.path == path / txtFilename);
    }

    SECTION("gif") {
        auto fileInfo = decoder.decodeFileInfo(path / gifFilename);
        REQUIRE(fileInfo.mimeFileType.typeGroup == "image");
        REQUIRE(fileInfo.mimeFileType.type == "gif");
        REQUIRE(fileInfo.path == path / gifFilename);
    }

    SECTION("png") {
        auto fileInfo = decoder.decodeFileInfo(path / pngFilename);
        REQUIRE(fileInfo.mimeFileType.typeGroup == "image");
        REQUIRE(fileInfo.mimeFileType.type == "png");
        REQUIRE(fileInfo.path == path / pngFilename);
    }

    SECTION("bad png") {
        auto fileInfo = decoder.decodeFileInfo(path / badPngFilename);
        REQUIRE(fileInfo.mimeFileType.typeGroup == "image");
        REQUIRE(fileInfo.mimeFileType.type == "png");
        REQUIRE(fileInfo.path == path / badPngFilename);
    }
}

TEST_CASE("FileMetaDataReader test") {
    //@todo use chrono ymd when it's available C++20
    testaid::TestDirWithResources resources(testaid::TestDirWithResources::Options{
            .fixedFileTime = std::chrono::file_clock::from_sys(std::chrono::system_clock::from_time_t(1586429253))});
    const auto& path              = resources.getResourcePath("mime");
    const std::string txtFilename = "test.txt";
    const std::string pngFilename = "test.png";
    MimeFileTypeDecoder decoder;
    SECTION("txt") {
        auto fileType = decoder.getTypeForFile(path / txtFilename);
        FileMetaDataReader fileMetaDataReader(fileType);
        const auto& meta = fileMetaDataReader.readMetaData(path / txtFilename);
        REQUIRE(meta["file"]["size"] == "11");
        REQUIRE(meta["file"]["date"]["year"].is_string());
        REQUIRE(meta["file"]["date"]["year"] == "2020");
        REQUIRE(meta["file"]["date"]["month"].is_string());
        REQUIRE(meta["file"]["date"]["month"] == "4");
        REQUIRE(meta["file"]["date"]["day"].is_string());
        REQUIRE(meta["file"]["date"]["day"] == "9");
        REQUIRE(meta["file"]["time"]["hours"].is_string());
        REQUIRE(meta["file"]["time"]["hours"] == "10");
        REQUIRE(meta["file"]["time"]["minutes"].is_string());
        REQUIRE(meta["file"]["time"]["minutes"] == "47");
        REQUIRE(meta["file"]["time"]["seconds"].is_string());
        REQUIRE(meta["file"]["time"]["seconds"] == "33");
        REQUIRE(meta["date"]["year"].is_string());
        REQUIRE(meta["date"]["year"] == "2020");
        REQUIRE(meta["date"]["month"].is_string());
        REQUIRE(meta["date"]["month"] == "4");
        REQUIRE(meta["date"]["day"].is_string());
        REQUIRE(meta["date"]["day"] == "9");
        REQUIRE(meta["time"]["hours"].is_string());
        REQUIRE(meta["time"]["hours"] == "10");
        REQUIRE(meta["time"]["minutes"].is_string());
        REQUIRE(meta["time"]["minutes"] == "47");
        REQUIRE(meta["time"]["seconds"].is_string());
        REQUIRE(meta["time"]["seconds"] == "33");
    }

    SECTION("png") {
        auto fileType = decoder.getTypeForFile(path / pngFilename);
        FileMetaDataReader fileMetaDataReader(fileType);
        const auto& meta = fileMetaDataReader.readMetaData(path / pngFilename);
        REQUIRE(meta["file"]["size"] == "8495");
        REQUIRE(meta["file"]["date"]["year"].is_string());
        REQUIRE(meta["file"]["date"]["year"] == "2020");
        REQUIRE(meta["file"]["date"]["month"].is_string());
        REQUIRE(meta["file"]["date"]["month"] == "4");
        REQUIRE(meta["file"]["date"]["day"].is_string());
        REQUIRE(meta["file"]["date"]["day"] == "9");
        REQUIRE(meta["file"]["time"]["hours"].is_string());
        REQUIRE(meta["file"]["time"]["hours"] == "10");
        REQUIRE(meta["file"]["time"]["minutes"].is_string());
        REQUIRE(meta["file"]["time"]["minutes"] == "47");
        REQUIRE(meta["file"]["time"]["seconds"].is_string());
        REQUIRE(meta["file"]["time"]["seconds"] == "33");
        REQUIRE(meta["image"]["date"]["year"].is_string());
        REQUIRE(meta["image"]["date"]["year"] == "2020");
        REQUIRE(meta["image"]["date"]["month"].is_string());
        //@todo I see inconsistency with leading 0s, either add them to file parser or remove from image parser (I think adding is better)
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