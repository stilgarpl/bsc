//
// Created by stilgar on 10.04.2020.
//

#include "Tester.h"
#include <random>
#include <utility>

namespace bsc {
    void Tester::createFile(const std::filesystem::path &path, const std::string &content) {
        std::ofstream file(path);
        file << content;
    }

    void Tester::changeFile(const fs::path &path, const std::string &content) {
        std::ofstream file(path);
        file << content;
    }

    std::string Tester::readFile(const fs::path &path) {
        if (fs::exists(path)) {
            std::ifstream t(path);
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            return str;
        } else {

            throw TestingException("file not found");
        }
    }

    Tester::Cleanup::Cleanup(fs::path pathToCleanup) : pathToCleanup(std::move(pathToCleanup)) {}

    Tester::Cleanup::~Cleanup() { fs::remove_all(pathToCleanup); }

    Tester::TestDir::TestDir(const fs::path &basePath) {
        if (!fs::exists(basePath)) {
            using namespace std::string_literals;
            throw TestingException("Base path "s + basePath.string() + "does not exist");
        }
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist;
        auto testDirName = std::to_string(dist(rng));
        while (fs::exists(basePath / testDirName)) { testDirName = std::to_string(dist(rng)); }
        path = basePath / testDirName;
        fs::create_directories(path);
        this->cleanup = std::make_unique<Cleanup>(path);
    }

    fs::path Tester::TestDir::getTestDirPath() { return path; }

    fs::path Tester::TestDir::getTestDirPath(const std::string &subdir) {
        auto testPath = path / subdir;
        fs::create_directories(testPath);
        return testPath;
    }

    TestingException::TestingException(const std::string& arg) : domain_error(arg) {}

    Tester::TestDirWithResources::TestDirWithResources(const fs::path& resourcesPath) {
        if (fs::exists(resourcesPath)) {
            fs::copy(resourcesPath, path, fs::copy_options::recursive);
        } else {
            throw TestingException("Resources path does not exist : " + resourcesPath.string());
        }
    }
    Tester::Resources::Resources(const fs::path& resourcePath) : resourcePath(resourcePath) {}
    const fs::path& Tester::Resources::getResourcePath() const { return resourcePath; }
    fs::path Tester::Resources::getResourcePath(const fs::path& p) const {
        //@todo maybe throw if this path does not exist?
        return resourcePath / p;
    }
}// namespace bsc
