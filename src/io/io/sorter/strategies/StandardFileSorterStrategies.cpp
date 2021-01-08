//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "StandardFileSorterStrategies.h"
#include <core/log/Logger.h>
#include <fmt/format.h>

#include <core/text/TextUtils.h>
#include <parser/parser/fromString.h>
namespace bsc {
    FileSortingStrategies::SortStrategy StandardFileSorterSortStrategies::copy = [](const std::filesystem::path& from,
                                                                                    const std::filesystem::path& to) {
        //@todo check for errors? throw ? anything? (copy may fail, permisions setting may fail, modification date may fail)
        LOGGER("Copying " + from.string() + " to " + to.string());
        LOGGER("Creating dirs: " + to.parent_path().string());
        auto modificationDate = fs::last_write_time(from);
        auto permissions      = fs::status(from).permissions();
        fs::create_directories(to.parent_path());
        fs::copy(from, to, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
        fs::last_write_time(to, modificationDate);
        fs::permissions(to, permissions);
    };
    FileSortingStrategies::SortStrategy StandardFileSorterSortStrategies::move = [](const fs::path& from,
                                                                                    const fs::path& to) {
        //@todo check for errors? throw ? anything?
        fs::create_directories(to.parent_path());
        fs::rename(from, to);
    };

    FileSortingStrategies::SortStrategy StandardFileSorterSortStrategies::erase = [](const fs::path& from,
                                                                                     const fs::path& to) {
        //@todo check for errors? throw ? anything?
        fs::remove(from);
    };
    FileSortingStrategies::SortStrategy StandardFileSorterSortStrategies::pretend = [](const fs::path& from,
                                                                                       const fs::path& to) {
        // do nothing
        //@todo this class could somehow return the list of files that it would process if it would do anything... maybe
        // it could take container by reference in the constructor and fill it?
    };

    FileSortingStrategies::CreateValidTargetPathStrategy StandardCreateValidTargetPathStrategies::overwrite =
            [](const fs::path& target, const FileSortingStrategies::FileExistsPredicate&) { return target; };

    FileSortingStrategies::CreateValidTargetPathStrategy StandardCreateValidTargetPathStrategies::skip =
            [](const fs::path& target, const FileSortingStrategies::FileExistsPredicate&) { return fs::path{}; };

    FileSortingStrategies::CreateValidTargetPathStrategy StandardCreateValidTargetPathStrategies::abort =
            [](const fs::path& target, const FileSortingStrategies::FileExistsPredicate&) -> fs::path {
        //@todo better error message
        throw FileSortingException("Aborting: " + target.string(), {}, target);
    };

    FileSortingStrategies::CreateValidTargetPathStrategy
    StandardCreateValidTargetPathStrategies::rename(const std::string& suffixFormat) {
        // calculate what possible here, so it doesn't have to be calculated when lambda is run.
        //@todo validate regex or add error handling.
        std::string regexString = fmt::format(TextUtils::escapeAllRegexCharacters(suffixFormat), "([0-9]+)") + '$';
        auto numberRegex        = std::regex(regexString);
        return [numberRegex, regexString, suffixFormat](const fs::path& target,
                                                        const FileSortingStrategies::FileExistsPredicate& fileExists) {
            // stem and update index to it's value

            int index         = 1;
            fs::path newPath  = target;
            auto parentPath   = target.parent_path();
            auto filenameStem = target.stem().string();
            std::smatch numberMatch;
            LOGGER("trying to match " + regexString + " to " + filenameStem);
            if (std::regex_search(filenameStem, numberMatch, numberRegex)) {
                auto numberString = std::string(numberMatch[1].first, numberMatch[1].second);
                LOGGER("Found number in name matching regex " + regexString + " with value " + numberString);
                index = fromString<int>(numberString);
                // remove matched part from string (it will be replaced in next step)
                filenameStem.erase(numberMatch[0].first, numberMatch[0].second);
            }
            do {
                auto suffix = fmt::format(suffixFormat, index);
                LOGGER("file name stem is :" + filenameStem);
                newPath = parentPath / (filenameStem + suffix + target.extension().string());
                LOGGER("rename: trying " + newPath.string())
                index++;
            } while (fileExists(newPath));
            return newPath;
        };
    }

    FileSortingStrategies::FileExistsPredicate StandardFileSorterPredicates::fileExistsPredicate =
            [](const fs::path& targetPath) { return fs::exists(targetPath); };

    class PretendFileExistsPredicate {
        std::set<fs::path> files;

    public:
        bool operator()(const fs::path& targetPath) {
            // static order initialization fiasco should not be a problem, this operator will be called long after it's
            // all initialized
            auto result = files.contains(targetPath) || StandardFileSorterPredicates::fileExistsPredicate(targetPath);
            files.insert(targetPath);
            return result;
        }
    };
    FileSortingStrategies::FileExistsPredicate StandardFileSorterPredicates::pretendFileExistsPredicate =
            PretendFileExistsPredicate();

    FileSortingStrategies::ErrorHandlingStrategy StandardFileSorterErrorHandlers::ignore =
            [](const fs::path& from, const std::exception& exception, std::list<SortFailure>& sortFailure) {
                ERROR("Failed processing " + from.string() + " exception: " + exception.what());
            };

    FileSortingStrategies::ErrorHandlingStrategy StandardFileSorterErrorHandlers::logAndContinue =
            [](const fs::path& from, const FileSortingException& exception, std::list<SortFailure>& sortFailure) {
                sortFailure.push_back({.sourcePath      = from,
                                       .destinationPath = exception.getDestinationPath(),
                                       .errorMessage    = exception.what()});
                ERROR("Failed processing " + from.string() + " exception: " + exception.what());
            };

    FileSortingStrategies::ErrorHandlingStrategy StandardFileSorterErrorHandlers::stop =
            [](const fs::path& from, const FileSortingException& exception, std::list<SortFailure>& sortFailure) {
                sortFailure.push_back({.sourcePath      = from,
                                       .destinationPath = exception.getDestinationPath(),
                                       .errorMessage    = exception.what()});
                ERROR("Failed processing " + from.string() + " exception: " + exception.what());
                throw exception;
            };
}// namespace bsc
