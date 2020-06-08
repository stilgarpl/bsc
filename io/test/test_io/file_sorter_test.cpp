//
// Created by stilgar on 20.04.2020.
//

#include <catch2/catch.hpp>
#include <io/sorter/FileSorter.h>
#include <io/sorter/fetchers/FilesystemFileListFetcher.h>
#include <io/sorter/fetchers/StaticFileListFetcher.h>
#include <io/sorter/mappers/FileSorterMapper.h>
#include <io/sorter/mappers/FileSorterMimeMatcher.h>
#include <io/sorter/mappers/FileSorterNameMatcher.h>
#include <io/sorter/strategies/StandardFileSorterStrategies.h>
#include <io/translator/PathTranslator.h>
#include <tester/Tester.h>

using namespace bsc;

TEST_CASE("Sort strategies test") {
    Tester::TestDirWithResources testDirWithResources;
    auto path    = testDirWithResources.getResourcePath("sort");
    auto newPath = path / "../other/";
    fs::create_directories(newPath);
    auto filename = "test.gif";
    SECTION("Copy") {
        auto& copyAction = StandardFileSorterSortStrategies::copy;
        copyAction(path / filename, newPath / filename);
        // assert that file was copied
        REQUIRE(fs::exists(newPath / filename));
        REQUIRE(!fs::is_directory(newPath / filename));
        REQUIRE(fs::exists(path / filename));
        REQUIRE(fs::file_size(newPath / filename) == fs::file_size(path / filename));
    }
    SECTION("Move") {
        auto& moveAction = StandardFileSorterSortStrategies::move;
        auto oldSize     = fs::file_size(path / filename);
        moveAction(path / filename, newPath / filename);
        // assert that file was moved
        REQUIRE(fs::exists(newPath / filename));
        REQUIRE(!fs::exists(path / filename));
        REQUIRE(fs::file_size(newPath / filename) == oldSize);
    }
    SECTION("Erase") {
        auto& eraseAction = StandardFileSorterSortStrategies::erase;
        eraseAction(path / filename, newPath / filename);
        // assert that file was deleted
        REQUIRE(!fs::exists(newPath / filename));
        REQUIRE(!fs::exists(path / filename));
    }
    SECTION("Pretend") {
        auto& pretendAction = StandardFileSorterSortStrategies::pretend;
        pretendAction(path / filename, newPath / filename);
        // assert that it didn't actually do anything
        REQUIRE(!fs::exists(newPath / filename));
        REQUIRE(fs::exists(path / filename));
    }
    //@todo add some tests for failed cases, files that not exist and so on.
}

TEST_CASE("Sort error strategies test") {
    const fs::path file = "/tmp";
    const FileSortingException exception("Exception");
    SECTION("ignore strategy") { REQUIRE_NOTHROW(StandardFileSorterErrorHandlers::ignore(file, exception)); }
    SECTION("stop strategy") {
        REQUIRE_THROWS_AS(StandardFileSorterErrorHandlers::stop(file, exception), FileSortingException);
    }
}

TEST_CASE("Sort predicates test") {
    Tester::TestDirWithResources testDirWithResources;
    auto resourcePath        = testDirWithResources.getResourcePath("sort");
    auto existingTestFile    = resourcePath / "test.gif";
    auto notExistingTestFile = resourcePath / "wrong.file";
    SECTION("file exists predicate") {
        auto predicate = StandardFileSorterPredicates::fileExistsPredicate;
        REQUIRE(predicate(existingTestFile) == true);
        REQUIRE(predicate(notExistingTestFile) == false);
        // does not change when asked again
        REQUIRE(predicate(existingTestFile) == true);
        REQUIRE(predicate(notExistingTestFile) == false);
    }

    SECTION("pretend file exists predicate") {
        auto predicate = StandardFileSorterPredicates::pretendFileExistsPredicate;
        REQUIRE(predicate(existingTestFile) == true);
        REQUIRE(predicate(notExistingTestFile) == false);
        // this predicate remembers all files that were
        REQUIRE(predicate(existingTestFile) == true);
        REQUIRE(predicate(notExistingTestFile) == true);
        SECTION("second time") {
            auto secondPredicate = StandardFileSorterPredicates::pretendFileExistsPredicate;
            auto predicateCopy   = predicate;
            REQUIRE(predicateCopy(existingTestFile) == true);
            REQUIRE(predicateCopy(notExistingTestFile) == true);
            // second predicate does not share memory with first
            REQUIRE(secondPredicate(existingTestFile) == true);
            REQUIRE(secondPredicate(notExistingTestFile) == false);
        }
    }
}

//@todo move this to more suitable place
TEST_CASE("escapeAllRegexCharacters test") {
    std::string text         = "[test(string).]";
    std::string expectedText = "\\[test\\(string\\)\\.\\]";
    auto result              = escapeAllRegexCharacters(text);
    REQUIRE(result == expectedText);
}

TEST_CASE("Sort fetchers test") {
    Tester::TestDirWithResources testDirWithResources;
    SECTION("Filesystem fetcher test") {
        std::set<fs::path> expectedResult = {"test.txt", "test.gif", "test.png", "png_with_bad_extension.txt"};
        auto path                         = testDirWithResources.getTestDirPath("sort");
        FilesystemFileListFetcher fetcher;
        auto result = fetcher.doListFiles(path);
        for (const auto& item : result) {
            REQUIRE(expectedResult.contains(item.filename()));
        }
    }

    SECTION("Static fetcher test ") {
        std::vector<fs::path> expectedResult = {"file1", "file2.txt", "file5.zip"};
        fs::path path                        = {"any/path"};
        StaticFileListFetcher fetcher(expectedResult);
        auto result = fetcher.doListFiles(path);
        REQUIRE(expectedResult == result);
    }
}

TEST_CASE("Sort mapper matchers test") {
    Tester::TestDirWithResources testDirWithResources;
    auto path     = testDirWithResources.getResourcePath("sort");
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
    SECTION("FileSorterNameMatcher") {
        SECTION("none") {
            auto expectedResult = MatchPrecision::none;
            FileSorterNameMatcher nameMatcher("(.)+\\.jpg");
            auto result = nameMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
        SECTION("perfect") {
            auto expectedResult = MatchPrecision::perfect;
            FileSorterNameMatcher nameMatcher("(.)+\\.gif");
            auto result = nameMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
    }
    //@todo implement
}

TEST_CASE("FileSorterMapper test") {
    Tester::TestDirWithResources testDirWithResources;
    auto path                       = testDirWithResources.getResourcePath("sort");
    auto pathSubdir                 = testDirWithResources.getResourcePath("sort/subdir");
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
    Tester::TestDirWithResources testDirWithResources;
    auto resourcePath    = testDirWithResources.getResourcePath("sort");
    auto destinationPath = testDirWithResources.getTestDirPath("destination");
    FileSorter fileSorter(std::make_unique<FilesystemFileListFetcher>(),
                          {.sortStrategy                  = StandardFileSorterSortStrategies::copy,
                           .createValidTargetPathStrategy = StandardCreateValidTargetPathStrategies::abort,
                           .errorHandlerStrategy          = StandardFileSorterErrorHandlers::stop,
                           .fileExistsPredicate           = StandardFileSorterPredicates::fileExistsPredicate

                          });
    SECTION("images only") {
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(MimeFileType::make("image/")),
                              destinationPath.string() + "/Images/{{date.year}}/");
        REQUIRE(fs::exists(resourcePath / "test.gif"));
        REQUIRE(fs::exists(resourcePath / "test.png"));
        REQUIRE(fs::exists(resourcePath / "png_with_bad_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "subdir" / "test.txt"));
        const auto& result = fileSorter.sort(resourcePath);
        REQUIRE(result.getFilesSortedMap().size() == 3);
        REQUIRE(fs::exists(destinationPath / "Images"));
        //@todo this will fail in 2021, fix tests with current year for images that does not have exif data
        REQUIRE(fs::exists(destinationPath / "Images" / "2020" / "test.gif"));
        REQUIRE(!fs::is_directory(destinationPath / "Images" / "2020" / "test.gif"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2020" / "test.png"));
        REQUIRE(!fs::is_directory(destinationPath / "Images" / "2020" / "test.png"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2020" / "png_with_bad_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "test.gif"));
        REQUIRE(fs::exists(resourcePath / "test.png"));
        REQUIRE(fs::exists(resourcePath / "png_with_bad_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "subdir" / "test.txt"));// was not moved
    }
    SECTION("images and txt") {
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(MimeFileType::make("image/")),
                              destinationPath.string() + "/Images/{{date.year}}/");
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(MimeFileType::make("text/")),
                              destinationPath.string() + "/Text/");
        REQUIRE(fs::exists(resourcePath / "test.gif"));
        REQUIRE(fs::exists(resourcePath / "test.png"));
        REQUIRE(fs::exists(resourcePath / "png_with_bad_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "subdir" / "test.txt"));
        const auto& result = fileSorter.sort(resourcePath);
        REQUIRE(result.getFilesSortedMap().size() == 4);
        REQUIRE(fs::exists(destinationPath / "Images"));
        //@todo this will fail in 2021, fix tests with current year for images that does not have exif data
        REQUIRE(fs::exists(destinationPath / "Images" / "2020" / "test.gif"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2020" / "test.png"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2020" / "png_with_bad_extension.txt"));
        REQUIRE(fs::exists(destinationPath / "Text" / "test.txt"));
        REQUIRE(fs::exists(resourcePath / "test.gif"));
        REQUIRE(fs::exists(resourcePath / "test.png"));
        REQUIRE(fs::exists(resourcePath / "png_with_bad_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "subdir" / "test.txt"));// was not moved
        SECTION("second sort should end with error") {
            const auto& result = fileSorter.sort(resourcePath);
            REQUIRE(result.getFilesSortedMap().size() == 0);
        }
    }
}

TEST_CASE("Target strategy test ") {
    Tester::TestDirWithResources testDirWithResources;
    auto resourcePath    = testDirWithResources.getResourcePath("sort");
    auto testGif         = resourcePath / "test.gif";
    const auto predicate = StandardFileSorterPredicates::fileExistsPredicate;
    REQUIRE(fs::exists(testGif));
    SECTION("overwrite") {
        const auto& action  = StandardCreateValidTargetPathStrategies::overwrite;
        auto expectedResult = testGif;
        auto result         = action(testGif, predicate);
        REQUIRE(result == testGif);
    }

    SECTION("rename") {
        const auto& action  = StandardCreateValidTargetPathStrategies::rename();
        auto expectedResult = resourcePath / "test (1).gif";
        SECTION("basic test") {
            auto result = action(testGif, predicate);
            REQUIRE(result == expectedResult);
            REQUIRE(!fs::is_directory(result));
            SECTION("second file") {
                Tester::createFile(expectedResult, "test");
                auto expectedSecondResult = resourcePath / "test (2).gif";
                auto result2              = action(testGif, predicate);
                REQUIRE(!fs::is_directory(result2));
                REQUIRE(result2 == expectedSecondResult);
            }
        }
        SECTION("number finding test") {
            const auto predicate2 = StandardFileSorterPredicates::pretendFileExistsPredicate;
            // this file does not exist, so it's ok, it won't be renamed, but other two should be.
            auto testGif9        = resourcePath / "test (9).gif";
            auto expectedResult1 = resourcePath / "test (9).gif";
            auto expectedResult2 = resourcePath / "test (10).gif";
            auto expectedResult3 = resourcePath / "test (11).gif";
            auto result          = action(testGif9, predicate2);
            auto result2         = action(testGif9, predicate2);
            auto result3         = action(testGif9, predicate2);
            REQUIRE(result == expectedResult1);
            REQUIRE(result2 == expectedResult2);
            REQUIRE(result3 == expectedResult3);
        }
    }

    SECTION("abort") {
        const auto& action = StandardCreateValidTargetPathStrategies::abort;
        REQUIRE_THROWS_AS(action(testGif, predicate), FileSortingException);
    }
}