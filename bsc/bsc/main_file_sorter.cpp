//
// Created by stilgar on 22.04.2020.
//
#include <filesystem>
#include <io/sorter/FileSorter.h>
#include <io/sorter/actions/StandardFileSorterActions.h>
#include <io/sorter/fetchers/FilesystemFileListFetcher.h>
#include <io/sorter/mappers/FileSorterMimeMatcher.h>
#include <io/sorter/mappers/FileSorterNameMatcher.h>
#include <parser/parameters/CommandLineParameters.h>
#include <string>
using namespace bsc;
using namespace std::string_literals;

int main(int argc, char* argv[]) {
    //@todo move actionFactory setup somewhere else
    static FileSorter::SortActionFactory actionFactory;
    actionFactory.addMold("copy", StandardFileSorterActions::copy);
    actionFactory.addMold("move", StandardFileSorterActions::move);
    actionFactory.addMold("pretend", StandardFileSorterActions::pretend);

    //@todo add way to list available actions in help
    struct FileSorterParameters : CommandLineParameters {
        Argument<std::filesystem::path> targetPath                        = {"PATH"};
        DefaultParameter<std::map<std::string, std::string>> mimeMatchers = {
                {.shortKey     = 'm',
                 .longKey      = "mime",
                 .argumentName = "mimetype=PATTERN",
                 .doc          = "Pair of mime type and path pattern"}};
        DefaultParameter<std::map<std::string, std::string>> nameMatchers = {
                {.shortKey     = 'n',
                 .longKey      = "name",
                 .argumentName = "regex=PATTERN",
                 .doc          = "Pair of filename regex and path pattern"}};

        DefaultParameter<std::string> action = {
                {.shortKey      = 'a',
                 .longKey       = "action",
                 .argumentName  = "ACTION",
                 .doc           = "Action to perform on files\nAvailable actions:\ncopy\nmove\npretend",
                 .defaultValue  = "copy",
                 .allowedValues = actionFactory.getMolds()}};
    };

    const auto& parameters = CommandLineParameters::parse<FileSorterParameters>(argc, argv);
    auto fetcher           = std::make_unique<FilesystemFileListFetcher>();
    FileSorter fileSorter(std::move(fetcher), actionFactory.create(parameters.action()));
    for (const auto& [mime, pattern] : parameters.mimeMatchers()) {
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(mime), pattern);
    }
    for (const auto& [name, pattern] : parameters.nameMatchers()) {
        fileSorter.addPattern(std::make_unique<FileSorterNameMatcher>(name), pattern);
    }
    fileSorter.sort(parameters.targetPath());
    return 0;
}