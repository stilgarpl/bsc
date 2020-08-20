//
// Created by Krzysztof Tulidowicz on 10.04.2020.
//
#include "testaid/testaid.h"
#include <catch2/catch.hpp>

using namespace bsc;

TEST_CASE("TestDir test") {
    SECTION("TestDir cleanup test ") {
        fs::path testPath;
        {
            testaid::TestDir testDir;
            testPath = testDir.getTestDirPath();
            REQUIRE(fs::exists(testPath));
        }
        REQUIRE(!fs::exists(testPath));
    }

    SECTION("TestDir cleanup test with custom subdir ") {
        fs::path testPath;
        {
            testaid::TestDir testDir;
            testPath = testDir.getTestDirPath("lala");
            REQUIRE(fs::exists(testPath));
            REQUIRE(testPath == testDir.getTestDirPath() / "lala");
        }
        REQUIRE(!fs::exists(testPath));
    }

    SECTION("TestDir create/read/cleanup") {
        fs::path testPath;
        const std::string filename = "file.txt";
        const std::string fileContent = "file content";
        {
            testaid::TestDir testDir;
            testPath = testDir.getTestDirPath();
            REQUIRE(fs::exists(testPath));
            testaid::createFile(testPath / filename, fileContent);
            REQUIRE(fs::exists(testPath / filename));
            auto readContent = testaid::readFile(testPath / filename);
            REQUIRE(readContent == fileContent);
        }
        REQUIRE(!fs::exists(testPath));
        REQUIRE(!fs::exists(testPath / filename));
    }

}

TEST_CASE("TestDirWithResources test") {
    fs::path testPath;
    {
        testaid::TestDirWithResources testDirWithResources;
        testPath = testDirWithResources.getResourcePath();
        REQUIRE(fs::exists(testPath));
        REQUIRE(fs::exists(testPath / "testFile1.txt"));
        REQUIRE(fs::exists(testPath / "testFile2.csv"));
        REQUIRE(testaid::readFile(testPath / "testFile1.txt") == "jfsdkjfhds");
    }
    REQUIRE(!fs::exists(testPath));
    REQUIRE(!fs::exists(testPath / "testFile1.txt"));
    REQUIRE(!fs::exists(testPath / "testFile2.csv"));
}

TEST_CASE("Resources test") {
    fs::path testPath;
    {
        testaid::Resources resources;
        const auto& resourcePath = resources.getResourcePath();
        REQUIRE(fs::exists(resourcePath));
        REQUIRE(fs::exists(resourcePath / "testFile1.txt"));
        REQUIRE(fs::exists(resourcePath / "testFile2.csv"));
        REQUIRE(testaid::readFile(resourcePath / "testFile1.txt") == "jfsdkjfhds");
    }
}