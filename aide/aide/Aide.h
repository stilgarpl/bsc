//
// Created by stilgar on 10.04.2020.
//

#ifndef BSC_AIDE_H
#define BSC_AIDE_H

#include <filesystem>
#include <fstream>

namespace bsc {
    namespace fs = std::filesystem;

    namespace aide {

        static void createFile(const fs::path& path, const std::string& content);

        static void changeFile(const fs::path& path, const std::string& content);

        static std::string readFile(const fs::path& path);

        class Cleanup {
            fs::path pathToCleanup;

        public:
            explicit Cleanup(fs::path pathToCleanup);

            virtual ~Cleanup();
        };

        class TestDir {
            std::unique_ptr<Cleanup> cleanup;

        protected:
            fs::path path;
            fs::path rootPath;

        public:
            fs::path getTestDirPath();

            explicit TestDir(const fs::path& basePath = fs::temp_directory_path());

            fs::path getTestDirPath(const std::string& subdir);
        };

        class TestDirWithResources : public TestDir {
        private:
            fs::path localResourcesPath;

        public:
            explicit TestDirWithResources(const fs::path& resourcesPath = "resources");
            [[nodiscard]] const fs::path& getResourcePath() const;
            [[nodiscard]] fs::path getResourcePath(const fs::path& p) const;
        };

        class Resources {
        private:
            fs::path resourcePath;

        public:
            explicit Resources(fs::path resourcePath = "resources");
            [[nodiscard]] const fs::path& getResourcePath() const;
            [[nodiscard]] fs::path getResourcePath(const fs::path&) const;
        };
    };

    class TestingException : public std::domain_error {
    public:
        explicit TestingException(const std::string& arg);
    };
}// namespace bsc

#endif