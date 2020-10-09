//
// Created by Krzysztof Tulidowicz on 10.04.2020.
//

#include "testaid.h"
#include <random>
#include <thread>
#include <utility>
using namespace std::chrono_literals;

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

    void waitFor(const std::function<bool(void)>& expression, std::chrono::milliseconds timeout) {

        auto beginTime = std::chrono::steady_clock::now();
        while (!expression() || std::chrono::steady_clock::now() - beginTime < timeout) {
            std::this_thread::sleep_for(1ms);
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

    TestDirWithResources::TestDirWithResources(Options options) {
        if (fs::exists(options.resourcesPath)) {
            localResourcesPath = rootPath / "resources";
            //@todo maybe it should not copy everything, but only requested files on getResourcePath ?
            fs::copy(options.resourcesPath, localResourcesPath, fs::copy_options::recursive);
            //@todo extract this to new function
            if (options.filePermissions || options.fixedFileTime) {
                for (const auto& item : fs::recursive_directory_iterator(localResourcesPath)) {
                    if (options.filePermissions) {
                        fs::permissions(item.path(), *options.filePermissions);
                    }
                    if (options.fixedFileTime) {
                        fs::last_write_time(item.path(), *options.fixedFileTime);
                    }
                }
            }
        } else {
            throw TestingException("Resources path does not exist : " + options.resourcesPath.string() +
                                   ", current dir: " + fs::current_path().string());
        }
        }
        const fs::path& TestDirWithResources::getResourcePath() const { return localResourcesPath; }
        fs::path TestDirWithResources::getResourcePath(const fs::path& p) const { return localResourcesPath / p; }
        TestDirWithResources::TestDirWithResources() : TestDirWithResources(Options()) {}
        Resources::Resources(fs::path resourcePath) : resourcePath(std::move(resourcePath)) {}
        const fs::path& Resources::getResourcePath() const { return resourcePath; }
        fs::path Resources::getResourcePath(const fs::path& p) const {
            //@todo maybe throw if this path does not exist?
            return resourcePath / p;
        }
}// namespace bsc::testaid
