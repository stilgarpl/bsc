//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include <catch2/catch.hpp>
#include <io/sorter/FileSorter.h>
#include <io/sorter/fetchers/FilesystemFileListFetcher.h>
#include <io/sorter/fetchers/StandardFileListFetcherConstraints.h>
#include <io/sorter/fetchers/StaticFileListFetcher.h>
#include <io/sorter/mappers/FileSorterMapper.h>
#include <io/sorter/mappers/FileSorterMimeMatcher.h>
#include <io/sorter/mappers/FileSorterNameMatcher.h>
#include <io/sorter/strategies/StandardFileSorterStrategies.h>
#include <io/translator/PathTranslator.h>
#include <testaid/testaid.h>

using namespace bsc;

TEST_CASE("Sort strategies test") {
    testaid::TestDirWithResources testDirWithResources;
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
    const FileSortingException exception("Exception", file);
    std::list<SortFailure> failures;
    SECTION("ignore strategy") {
        REQUIRE_NOTHROW(StandardFileSorterErrorHandlers::ignore(file, exception, failures));
        REQUIRE(failures.empty());
    }
    SECTION("continue strategy") {
        REQUIRE_NOTHROW(StandardFileSorterErrorHandlers::logAndContinue(file, exception, failures));
        REQUIRE(!failures.empty());
    }
    SECTION("stop strategy") {
        REQUIRE_THROWS_AS(StandardFileSorterErrorHandlers::stop(file, exception, failures), FileSortingException);
        REQUIRE(!failures.empty());
    }
}

TEST_CASE("Sort predicates test") {
    testaid::TestDirWithResources testDirWithResources;
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

TEST_CASE("Sort fetchers test") {
    testaid::TestDirWithResources testDirWithResources;
    SECTION("Filesystem fetcher test") {
        std::set<fs::path> expectedResult = {"test.txt", "test.gif", "test.png", "png_with_wrong_extension.txt"};
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
    testaid::TestDirWithResources testDirWithResources;
    MimeFileTypeFactory factory;
    auto path     = testDirWithResources.getResourcePath("sort");
    auto filename = "test.gif";
    SECTION("FileSorterMimeMatcher") {
        SECTION("none - full") {
            auto expectedResult = MatchPrecision::none;
            FileSorterMimeMatcher mimeMatcher(factory.create("text/plain"));

            auto result = mimeMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
        SECTION("none - partial") {
            auto expectedResult = MatchPrecision::none;
            FileSorterMimeMatcher mimeMatcher(factory.create("text/"));

            auto result = mimeMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
        SECTION("partial") {
            auto expectedResult = MatchPrecision::partial;
            FileSorterMimeMatcher mimeMatcher(factory.create("image/"));

            auto result = mimeMatcher.matches(path / filename);
            REQUIRE(result == expectedResult);
        }
        SECTION("perfect") {
            auto expectedResult = MatchPrecision::perfect;
            FileSorterMimeMatcher mimeMatcher(factory.create("image/gif"));

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
}

TEST_CASE("FileSorterMapper test") {
    testaid::TestDirWithResources testDirWithResources;
    MimeFileTypeFactory factory;
    auto path                       = testDirWithResources.getResourcePath("sort");
    auto pathSubdir                 = testDirWithResources.getResourcePath("sort/subdir");
    const auto expectedTextPattern  = "text";
    const auto expectedImagePattern = "img";
    const auto expectedGifPattern   = "gif";

    FileSorterMapper fileSorterMapperEmpty;
    FileSorterMapper fileSorterMapper;
    fileSorterMapper.addPattern(std::make_unique<FileSorterMimeMatcher>(factory.create("text/")), expectedTextPattern);
    fileSorterMapper.addPattern(std::make_unique<FileSorterMimeMatcher>(factory.create("image/")), expectedImagePattern);
    fileSorterMapper.addPattern(std::make_unique<FileSorterMimeMatcher>(factory.create("image/gif")), expectedGifPattern);
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
        auto filenamePngTxt = "png_with_wrong_extension.txt";
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
    testaid::TestDirWithResources testDirWithResources;
    MimeFileTypeFactory factory;
    auto resourcePath    = testDirWithResources.getResourcePath("sort");
    auto destinationPath = testDirWithResources.getTestDirPath("destination");
    FileSorter fileSorter(std::make_unique<FilesystemFileListFetcher>(),
                          {.sortStrategy                  = StandardFileSorterSortStrategies::copy,
                           .createValidTargetPathStrategy = StandardCreateValidTargetPathStrategies::abort,
                           .errorHandlerStrategy          = StandardFileSorterErrorHandlers::stop,
                           .fileExistsPredicate           = StandardFileSorterPredicates::fileExistsPredicate

                          });
    SECTION("images only") {
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(factory.create("image/")),
                              destinationPath.string() + "/Images/{{date.year}}/");
        REQUIRE(fs::exists(resourcePath / "test.gif"));
        REQUIRE(fs::exists(resourcePath / "test.png"));
        REQUIRE(fs::exists(resourcePath / "png_with_wrong_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "subdir" / "test.txt"));
        const auto& result = fileSorter.sort({resourcePath});
        REQUIRE(result.getSortedFiles().size() == 3);
        REQUIRE(fs::exists(destinationPath / "Images"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2021" / "test.gif"));
        REQUIRE(!fs::is_directory(destinationPath / "Images" / "2021" / "test.gif"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2020" / "test.png"));
        REQUIRE(!fs::is_directory(destinationPath / "Images" / "2020" / "test.png"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2021" / "png_with_wrong_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "test.gif"));
        REQUIRE(fs::exists(resourcePath / "test.png"));
        REQUIRE(fs::exists(resourcePath / "png_with_wrong_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "subdir" / "test.txt"));// was not moved
    }
    SECTION("images and txt") {
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(factory.create("image/")),
                              destinationPath.string() + "/Images/{{date.year}}/");
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(factory.create("text/")), destinationPath.string() + "/Text/");
        REQUIRE(fs::exists(resourcePath / "test.gif"));
        REQUIRE(fs::exists(resourcePath / "test.png"));
        REQUIRE(fs::exists(resourcePath / "png_with_wrong_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "subdir" / "test.txt"));
        const auto& result = fileSorter.sort({resourcePath});
        REQUIRE(result.getSortedFiles().size() == 4);
        REQUIRE(fs::exists(destinationPath / "Images"));
        //@todo replace "2020" with current year, "2020" in png stays, it's taken from exif
        REQUIRE(fs::exists(destinationPath / "Images" / "2021" / "test.gif"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2020" / "test.png"));
        REQUIRE(fs::exists(destinationPath / "Images" / "2021" / "png_with_wrong_extension.txt"));
        REQUIRE(fs::exists(destinationPath / "Text" / "test.txt"));
        REQUIRE(fs::exists(resourcePath / "test.gif"));
        REQUIRE(fs::exists(resourcePath / "test.png"));
        REQUIRE(fs::exists(resourcePath / "png_with_wrong_extension.txt"));
        REQUIRE(fs::exists(resourcePath / "subdir" / "test.txt"));// was not moved
        SECTION("second sort should end with error") {
            const auto& result = fileSorter.sort({resourcePath});
            REQUIRE(result.getSortedFiles().size() == 0);
        }
    }
}

TEST_CASE("Target strategy test ") {
    testaid::TestDirWithResources testDirWithResources;
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
                testaid::createFile(expectedResult, "test");
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

TEST_CASE("Matchers test") {
    SECTION("is") {
        auto isBb = StandardFileListFetcherConstraints::is("bb");
        REQUIRE(isBb("bb") == true);
        REQUIRE(isBb("") == false);
        REQUIRE(isBb("aa") == false);
    }

    SECTION("time") {
        using namespace std::chrono_literals;
        auto now    = std::chrono::steady_clock::now();
        auto past   = now - 1h;
        auto future = now + 1h;
        SECTION("before") {
            auto isBefore = StandardFileListFetcherConstraints::before(now);
            REQUIRE(isBefore(past) == true);
            REQUIRE(isBefore(future) == false);
        }
        SECTION("after") {
            auto isAfter = StandardFileListFetcherConstraints::after(now);
            REQUIRE(isAfter(past) == false);
            REQUIRE(isAfter(future) == true);
        }
    }
}

TEST_CASE("Constraints test") {

    fs::path testPath    = "/test/path";
    fs::path otherPath   = "/test/path/1";
    fs::path testPathExt = "/test/path.exe";
    SECTION("path") {
        auto con = StandardFileListFetcherConstraints::path([testPath](const auto& p) { return p == testPath; });
        REQUIRE(con(testPath) == true);
        REQUIRE(con(otherPath) == false);
    }

    SECTION("filename") {
        auto con = StandardFileListFetcherConstraints::filename([testPath](const auto& p) { return p == "path"; });
        REQUIRE(con(testPath) == true);
        REQUIRE(con(otherPath) == false);
    }

    SECTION("extension") {
        auto con = StandardFileListFetcherConstraints::extension([testPath](const auto& p) { return p == ".exe"; });
        REQUIRE(con(testPathExt) == true);
        REQUIRE(con(otherPath) == false);
    }

    SECTION("date") {
        testaid::TestDirWithResources testDirWithResources;
        auto realPath = testDirWithResources.getResourcePath("sort") / "test.gif";
        auto con      = StandardFileListFetcherConstraints::date([realPath](const auto& p) { return p == fs::last_write_time(realPath); });
        REQUIRE(con(realPath) == true);
    }
}

#include <experimental/source_location>

TEST_CASE("Matchers and constraints test") {
    using namespace StandardFileListFetcherConstraints;
    SECTION("path - is ") {
        fs::path testPath  = "/test/path";
        fs::path otherPath = "/test/path/1";
        auto p             = path(is(testPath));
        REQUIRE(p(testPath) == true);
        REQUIRE(p(otherPath) == false);
    }

    SECTION("date - before ") {
        using namespace std::chrono_literals;
        testaid::TestDirWithResources testDirWithResources;
        auto realPath = testDirWithResources.getResourcePath("sort") / "test.gif";
        auto p1       = date(before(fs::last_write_time(realPath) + 1h));
        auto p2       = date(before(fs::last_write_time(realPath) - 1h));
        REQUIRE(p1(realPath) == true);
        REQUIRE(p2(realPath) == false);
    }
}