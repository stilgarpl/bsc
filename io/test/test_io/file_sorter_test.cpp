//
// Created by stilgar on 20.04.2020.
//

#include <catch2/catch.hpp>
#include <io/sorter/actions/StandardFileSorterActions.h>
#include <io/sorter/fetchers/FilesystemFileListFetcher.h>
#include <io/sorter/fetchers/StaticFileListFetcher.h>
#include <io/sorter/mappers/FileSorterMapper.h>
#include <io/sorter/mappers/FileSorterMimeMatcher.h>
#include <io/translator/PathTranslator.h>
#include <tester/Tester.h>

using namespace bsc;

TEST_CASE("Sort actions test") {
    Tester::TestDirWithResources testDirWithResources;
    auto path    = testDirWithResources.getTestDirPath("sort");
    auto newPath = path / "../other/";
    fs::create_directories(newPath);
    auto filename = "test.gif";
    SECTION("Copy") {
        StandardFileSorterActions::Copy copyAction;
        copyAction.sort(path / filename, newPath / filename);
        // assert that file was copied
        REQUIRE(fs::exists(newPath / filename));
        REQUIRE(fs::exists(path / filename));
        REQUIRE(fs::file_size(newPath / filename) == fs::file_size(path / filename));
    }
    SECTION("Move") {
        StandardFileSorterActions::Move moveAction;
        auto oldSize = fs::file_size(path / filename);
        moveAction.sort(path / filename, newPath / filename);
        // assert that file was moved
        REQUIRE(fs::exists(newPath / filename));
        REQUIRE(!fs::exists(path / filename));
        REQUIRE(fs::file_size(newPath / filename) == oldSize);
    }
    SECTION("Pretend") {
        StandardFileSorterActions::Pretend pretendAction;
        pretendAction.sort(path / filename, newPath / filename);
        // assert that it didn't actually do anything
        REQUIRE(!fs::exists(newPath / filename));
        REQUIRE(fs::exists(path / filename));
    }
    //@todo add some tests for failed cases, files that not exist and so on.
}

TEST_CASE("Sort fetchers test") {
    Tester::TestDirWithResources testDirWithResources;
    SECTION("Filesystem fetcher test") {
        std::set<fs::path> expectedResult = {"test.txt", "test.gif", "test.png", "png_with_bad_extension.txt"};
        auto path                         = testDirWithResources.getTestDirPath("sort");
        FilesystemFileListFetcher fetcher;
        auto result = fetcher.listFiles(path);
        for (const auto& item : result) {
            REQUIRE(expectedResult.contains(item.filename()));
        }
    }

    SECTION("Static fetcher test ") {
        std::vector<fs::path> expectedResult = {"file1", "file2.txt", "file5.zip"};
        fs::path path                        = {"any/path"};
        StaticFileListFetcher fetcher(expectedResult);
        auto result = fetcher.listFiles(path);
        REQUIRE(expectedResult == result);
    }
}

TEST_CASE("Sort mapper matchers test") {
    Tester::TestDirWithResources testDirWithResources;
    auto path     = testDirWithResources.getTestDirPath("sort");
    auto filename = "test.gif";
    SECTION("FileSorterMimeMatcher") {
        SECTION("none - full") {
            auto expectedResult = MatchPrecision::none;
            FileSorterMimeMatcher mimeMatcher(MimeFileType::make("text/plain"));

            auto result = mimeMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
        SECTION("none - partial") {
            auto expectedResult = MatchPrecision::none;
            FileSorterMimeMatcher mimeMatcher(MimeFileType::make("text/"));

            auto result = mimeMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
        SECTION("partial") {
            auto expectedResult = MatchPrecision::partial;
            FileSorterMimeMatcher mimeMatcher(MimeFileType::make("image/"));

            auto result = mimeMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
        SECTION("perfect") {
            auto expectedResult = MatchPrecision::perfect;
            FileSorterMimeMatcher mimeMatcher(MimeFileType::make("image/gif"));

            auto result = mimeMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
    }
    //@todo implement
}

TEST_CASE("FileSorterMapper test") {
    Tester::TestDirWithResources testDirWithResources;
    auto path                       = testDirWithResources.getTestDirPath("sort");
    auto pathSubdir                 = testDirWithResources.getTestDirPath("sort/subdir");
    const auto expectedTextPattern  = "text";
    const auto expectedImagePattern = "img";
    const auto expectedGifPattern   = "gif";

    FileSorterMapper fileSorterMapperEmpty;
    FileSorterMapper fileSorterMapper;
    fileSorterMapper.addPattern(std::make_unique<FileSorterMimeMatcher>(MimeFileType::make("text/")),
                                expectedTextPattern);
    fileSorterMapper.addPattern(std::make_unique<FileSorterMimeMatcher>(MimeFileType::make("image/")),
                                expectedImagePattern);
    fileSorterMapper.addPattern(std::make_unique<FileSorterMimeMatcher>(MimeFileType::make("image/gif")),
                                expectedGifPattern);
    SECTION("gif") {
        auto filenameGif = "test.gif";
        auto result      = fileSorterMapper.map(path / filenameGif);
        REQUIRE(result);
        REQUIRE(result == expectedGifPattern);
    }
    SECTION("png") {
        auto filenamePng = "test.png";
        auto result      = fileSorterMapper.map(path / filenamePng);
        REQUIRE(result);
        REQUIRE(result == expectedImagePattern);
    }
    SECTION("png-txt") {
        auto filenamePngTxt = "png_with_bad_extension.txt";
        auto result         = fileSorterMapper.map(path / filenamePngTxt);
        REQUIRE(result);
        REQUIRE(result == expectedImagePattern);
    }

    SECTION("txt") {
        auto filenameTxt = "test.txt";
        auto result      = fileSorterMapper.map(pathSubdir / filenameTxt);
        REQUIRE(result);
        REQUIRE(result == expectedTextPattern);
    }

    SECTION("no mapper") {

        auto filenameGif = "test.gif";
        auto result      = fileSorterMapperEmpty.map(path / filenameGif);
        REQUIRE(!result.has_value());
    }
}

TEST_CASE("Path translator test") {
    PathTranslator pathTranslator;
    PropertiesMetaData properties;
    SECTION("images pattern") {
        std::string pattern     = "{{homedir}}/{{imagesdir}}";
        properties["homedir"]   = "/home/test";
        properties["imagesdir"] = "Images";
        fs::path expectedPath   = "/home/test/Images";
        const auto& result      = pathTranslator.translate(pattern, properties);
        REQUIRE(result == expectedPath);
    }

    SECTION("invalid property pattern") {
        std::string pattern   = "{{homedir}}/{{imagesdir}}";
        properties["homedir"] = "/home/test";
        fs::path expectedPath = "/home/test/imagesdir";
        REQUIRE_THROWS_AS(pathTranslator.translate(pattern, properties), PathTranslationException);
    }
}

TEST_CASE("File sorter test") {
    //@todo implement
}
