//
// Created by stilgar on 10.04.2020.
//
#include <catch2/catch.hpp>
#include "tester/Tester.h"

using namespace bsc;

TEST_CASE("TestDir test") {
    SECTION("TestDir cleanup test ") {
        fs::path testPath;
        {
            Tester::TestDir testDir;
            testPath = testDir.getTestDirPath();
            REQUIRE(fs::exists(testPath));
        }
        REQUIRE(!fs::exists(testPath));
    }

    SECTION("TestDir cleanup test with custom subdir ") {
        fs::path testPath;
        {
            Tester::TestDir testDir;
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
            Tester::TestDir testDir;
            testPath = testDir.getTestDirPath();
            REQUIRE(fs::exists(testPath));
            Tester::createFile(testPath / filename, fileContent);
            REQUIRE(fs::exists(testPath / filename));
            auto readContent = Tester::readFile(testPath / filename);
            REQUIRE(readContent == fileContent);
        }
        REQUIRE(!fs::exists(testPath));
        REQUIRE(!fs::exists(testPath / filename));
    }

}

TEST_CASE("TestDirWithResources test") {
    fs::path testPath;
    {
        Tester::TestDirWithResources testDirWithResources;
        testPath = testDirWithResources.getTestDirPath();
        REQUIRE(fs::exists(testPath));
        REQUIRE(fs::exists(testPath / "testFile1.txt"));
        REQUIRE(fs::exists(testPath / "testFile2.csv"));
        REQUIRE(Tester::readFile(testPath / "testFile1.txt") == "jfsdkjfhds");
    }
    REQUIRE(!fs::exists(testPath));
    REQUIRE(!fs::exists(testPath / "testFile1.txt"));
    REQUIRE(!fs::exists(testPath / "testFile2.csv"));

}