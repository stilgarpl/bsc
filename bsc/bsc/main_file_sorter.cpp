//
// Created by stilgar on 22.04.2020.
//
#include <filesystem>
#include <io/sorter/FileSorter.h>
#include <io/sorter/actions/StandardFileSorterActions.h>
#include <io/sorter/fetchers/FilesystemFileListFetcher.h>
#include <io/sorter/mappers/FileSorterMimeMatcher.h>
#include <parser/parameters/CommandLineParameters.h>
#include <string>
using namespace bsc;
using namespace std::string_literals;
//@todo add action - copy, move, pretend
struct FileSorterParameters : CommandLineParameters {
    Argument<std::filesystem::path> targetPath                        = {"PATH"};
    DefaultParameter<std::map<std::string, std::string>> mimeMatchers = {'m',
                                                                         "mime",
                                                                         "PATTERN",
                                                                         "Pair of mime type and path pattern",
                                                                         {}};
};
int main(int argc, char* argv[]) {

    const auto& parameters = CommandLineParameters::parse<FileSorterParameters>(argc, argv);
    auto fetcher           = std::make_unique<FilesystemFileListFetcher>();
    FileSorter fileSorter(std::move(fetcher), StandardFileSorterActions::copy);
    for (const auto& [mime, pattern] : parameters.mimeMatchers()) {
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(mime), pattern);
    }
    fileSorter.sort(parameters.targetPath());
    return 0;
}