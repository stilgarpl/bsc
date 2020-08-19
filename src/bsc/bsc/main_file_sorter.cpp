//
// Created by Krzysztof Tulidowicz on 22.04.2020.
//
#include <filesystem>
#include <io/sorter/FileSorter.h>
#include <io/sorter/fetchers/FilesystemFileListFetcher.h>
#include <io/sorter/mappers/FileSorterMimeMatcher.h>
#include <io/sorter/mappers/FileSorterNameMatcher.h>
#include <io/sorter/strategies/StandardFileSorterStrategies.h>
#include <parser/parameters/CommandLineParameters.h>
#include <string>
using namespace bsc;
using namespace std::string_literals;

int main(int argc, char* argv[]) {
    //@todo move actionFactory setup somewhere else
    static FileSortingStrategyFactories::SortStrategyFactory actionFactory;
    actionFactory.registerCreator("copy", StandardFileSorterSortStrategies::copy);
    actionFactory.registerCreator("move", StandardFileSorterSortStrategies::move);
    actionFactory.registerCreator("pretend", StandardFileSorterSortStrategies::pretend);
    static FileSortingStrategyFactories::ErrorActionFactory errorActionFactory;
    errorActionFactory.registerCreator("ignore", StandardFileSorterErrorHandlers::ignore);
    errorActionFactory.registerCreator("continue", StandardFileSorterErrorHandlers::logAndContinue);
    errorActionFactory.registerCreator("stop", StandardFileSorterErrorHandlers::stop);
    static FileSortingStrategyFactories::CreateValidTargetPathStrategyFactory fileExistsFactory;
    fileExistsFactory.registerCreator("overwrite", StandardCreateValidTargetPathStrategies::overwrite);
    fileExistsFactory.registerCreator("skip", StandardCreateValidTargetPathStrategies::skip);
    fileExistsFactory.registerCreator("abort", StandardCreateValidTargetPathStrategies::abort);
    fileExistsFactory.registerCreator("rename", StandardCreateValidTargetPathStrategies::rename);

    //@todo add way to list available strategies in help
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
                 .doc           = "Action to perform on files\nAvailable actions:\ncopy *\nmove\npretend",
                 .defaultValue  = "copy",
                 .allowedValues = actionFactory.getSelectors()}};
        DefaultParameter<std::string> errorHandler = {
                {.shortKey      = 'e',
                 .longKey       = "error",
                 .argumentName  = "ERROR HANDLER",
                 .doc           = "What to do when error occurs.\nAvailable handlers:\nstop *\nignore",
                 .defaultValue  = "stop",
                 .allowedValues = errorActionFactory.getSelectors()}};
        DefaultParameter<std::string> fileExists = {
                {.shortKey     = 'f',
                 .longKey      = "file-exists",
                 .argumentName = "ACTION",
                 .doc = "What to do when target file exists.\nAvailable actions:\nrename *\nskip\noverwrite\nabort",
                 .defaultValue  = "rename",
                 .allowedValues = fileExistsFactory.getSelectors()}};
    };

    const auto& parameters = CommandLineParser::defaultParse<FileSorterParameters>(argc, argv);
    auto fetcher           = std::make_unique<FilesystemFileListFetcher>();
    FileSorter fileSorter(std::move(fetcher),
                          {.sortStrategy                  = actionFactory.create(parameters.action(), {}),
                           .createValidTargetPathStrategy = fileExistsFactory.create(parameters.fileExists(), {}),
                           .errorHandlerStrategy          = errorActionFactory.create(parameters.errorHandler(), {}),
                           .fileExistsPredicate           = StandardFileSorterPredicates::fileExistsPredicate});
    for (const auto& [mime, pattern] : parameters.mimeMatchers()) {
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(mime), pattern);
    }
    for (const auto& [name, pattern] : parameters.nameMatchers()) {
        fileSorter.addPattern(std::make_unique<FileSorterNameMatcher>(name), pattern);
    }
    fileSorter.sort(parameters.targetPath()) | StandardResultConsumers::printResult;
    return 0;
}