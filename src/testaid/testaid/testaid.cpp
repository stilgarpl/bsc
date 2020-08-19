//
// Created by Krzysztof Tulidowicz on 10.04.2020.
//

#include "testaid.h"
#include <random>
#include <utility>

namespace bsc::testaid {
    void createFile(const std::filesystem::path& path, const std::string& content) {
        std::ofstream file(path);
        file << content;
    }

    void changeFile(const fs::path& path, const std::string& content) {
        std::ofstream file(path);
        file << content;
    }

    std::string readFile(const fs::path& path) {
            if (fs::exists(path)) {
                std::ifstream t(path);
                std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
                return str;
            } else {

                throw TestingException("file not found");
            }
        }

        Cleanup::Cleanup(fs::path pathToCleanup) : pathToCleanup(std::move(pathToCleanup)) {}

        Cleanup::~Cleanup() { fs::remove_all(pathToCleanup); }

        TestDir::TestDir(const fs::path& basePath) {
            if (!fs::exists(basePath)) {
                using namespace std::string_literals;
                throw TestingException("Base path "s + basePath.string() + "does not exist");
            }
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist;
            const std::string rootPrefix = "bsc-test-";
            fs::path testDirName;
            do {
                testDirName = rootPrefix + std::to_string(dist(rng));
            } while (fs::exists(basePath / testDirName));
            rootPath = basePath / testDirName;
            path     = rootPath / "test";
            fs::create_directories(path);
            this->cleanup = std::make_unique<Cleanup>(rootPath);
        }

        fs::path TestDir::getTestDirPath() { return path; }

        fs::path TestDir::getTestDirPath(const std::string& subdir) {
            auto testPath = path / subdir;
            fs::create_directories(testPath);
            return testPath;
        }

        TestingException::TestingException(const std::string& arg) : domain_error(arg) {}

        TestDirWithResources::TestDirWithResources(const fs::path& resourcesPath) {
            if (fs::exists(resourcesPath)) {
                localResourcesPath = rootPath / "resources";
                //@todo maybe it should not copy everything, but only requested files on getResourcePath ?
                fs::copy(resourcesPath, localResourcesPath, fs::copy_options::recursive);
            } else {
                throw TestingException("Resources path does not exist : " + resourcesPath.string());
            }
        }
        const fs::path& TestDirWithResources::getResourcePath() const { return localResourcesPath; }
        fs::path TestDirWithResources::getResourcePath(const fs::path& p) const { return localResourcesPath / p; }
        Resources::Resources(fs::path resourcePath) : resourcePath(std::move(resourcePath)) {}
        const fs::path& Resources::getResourcePath() const { return resourcePath; }
        fs::path Resources::getResourcePath(const fs::path& p) const {
            //@todo maybe throw if this path does not exist?
            return resourcePath / p;
        }
}// namespace bsc::testaid
