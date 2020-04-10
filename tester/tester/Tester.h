//
// Created by stilgar on 10.04.2020.
//

#ifndef BSC_TESTER_H
#define BSC_TESTER_H

#include <filesystem>
#include <fstream>

namespace bsc {
    namespace fs = std::filesystem;

    class Tester {
    public:
        static void createFile(const fs::path &path, const std::string &content);

        static void changeFile(const fs::path &path, const std::string &content);

        static std::string readFile(const fs::path &path);

        class Cleanup {
            fs::path pathToCleanup;

        public:
            Cleanup(fs::path pathToCleanup);

            virtual ~Cleanup();
        };

        class TestDir {
            std::unique_ptr<Cleanup> cleanup;

        protected:
            fs::path path;

        public:
            fs::path getTestDirPath();

            TestDir(const fs::path &basePath = fs::temp_directory_path());

            fs::path getTestDirPath(const std::string &subdir);
        };

        class TestDirWithResources : public TestDir {
        public:
            TestDirWithResources(const fs::path &resourcesPath = "resources");
        };
    };

    class TestingException : public std::domain_error {
    public:
        TestingException(const std::string &arg);
    };
}// namespace bsc

#endif