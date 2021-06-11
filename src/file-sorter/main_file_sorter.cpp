//
// Created by Krzysztof Tulidowicz on 22.04.2020.
//
#include <filesystem>
#include <io/sorter/FileSorter.h>
#include <io/sorter/fetchers/FilesystemFileListFetcher.h>
#include <io/sorter/fetchers/FileListFetcher.h>
#include <io/sorter/mappers/FileSorterMimeMatcher.h>
#include <io/sorter/mappers/FileSorterNameMatcher.h>
#include <io/sorter/strategies/StandardFileSorterStrategies.h>
#include <parser/parameters/CommandLineParameters.h>
#include <string>
using namespace bsc;
using namespace std::string_literals;

auto makeActionFactory() {
    static FileSortingStrategyFactories::SortStrategyFactory actionFactory;
    actionFactory.registerCreator("copy", StandardFileSorterSortStrategies::copy);
    actionFactory.registerCreator("move", StandardFileSorterSortStrategies::move);
    actionFactory.registerCreator("pretend", StandardFileSorterSortStrategies::pretend);
    return actionFactory;
}

auto makeErrorActionFactory() {
    static FileSortingStrategyFactories::ErrorActionFactory errorActionFactory;
    errorActionFactory.registerCreator("ignore", StandardFileSorterErrorHandlers::ignore);
    errorActionFactory.registerCreator("continue", StandardFileSorterErrorHandlers::logAndContinue);
    errorActionFactory.registerCreator("stop", StandardFileSorterErrorHandlers::stop);
    return errorActionFactory;
}

auto makeFileExistsFactory() {
    static FileSortingStrategyFactories::CreateValidTargetPathStrategyFactory fileExistsFactory;
    fileExistsFactory.registerCreator("overwrite", StandardCreateValidTargetPathStrategies::overwrite);
    fileExistsFactory.registerCreator("skip", StandardCreateValidTargetPathStrategies::skip);
    fileExistsFactory.registerCreator("abort", StandardCreateValidTargetPathStrategies::abort);
    //@todo maybe rename should be renamed to "addSuffix", because that's what it does. rename could be added as search-and-replace regex
    fileExistsFactory.registerCreator("rename", StandardCreateValidTargetPathStrategies::rename);
    return fileExistsFactory;
}

int main(int argc, char* argv[]) {
    auto context = Context::makeContext();
    Context::setActiveContext(context);
    static FileSortingStrategyFactories::SortStrategyFactory actionFactory                      = makeActionFactory();
    static FileSortingStrategyFactories::ErrorActionFactory errorActionFactory                  = makeErrorActionFactory();
    static FileSortingStrategyFactories::CreateValidTargetPathStrategyFactory fileExistsFactory = makeFileExistsFactory();
    //@todo add way to list available strategies in help
    struct FileSorterParameters : CommandLineParameters {
        Arguments<std::vector<std::filesystem::path>> targetPaths         = {"PATHS..."};
        DefaultParameter<std::map<std::string, std::string>> mimeMatchers = {
                {.shortKey = 'm', .longKey = "mime", .argumentName = "mimetype=PATTERN", .doc = "Pair of mime type and path pattern"}};
        DefaultParameter<std::map<std::string, std::string>> nameMatchers = {
                {.shortKey = 'n', .longKey = "name", .argumentName = "regex=PATTERN", .doc = "Pair of filename regex and path pattern"}};

        DefaultParameter<std::string> action       = {{.shortKey      = 'a',
                                                 .longKey       = "action",
                                                 .argumentName  = "ACTION",
                                                 .doc           = "Action to perform on files\nAvailable actions:\ncopy *\nmove\npretend",
                                                 .defaultValue  = "copy",
                                                 .allowedValues = actionFactory.getSelectors()}};
        DefaultParameter<std::string> errorHandler = {{.shortKey     = 'e',
                                                       .longKey      = "error",
                                                       .argumentName = "ERROR HANDLER",
                                                       .doc          = "What to do when error occurs.\nAvailable handlers:\nstop *\nignore",
                                                       .defaultValue = "stop",
                                                       .allowedValues = errorActionFactory.getSelectors()}};
        DefaultParameter<std::string> fileExists   = {
                {.shortKey      = 'f',
                 .longKey       = "file-exists",
                 .argumentName  = "ACTION",
                 .doc           = "What to do when target file exists.\nAvailable actions:\nrename *\nskip\noverwrite\nabort",
                 .defaultValue  = "rename",
                 .allowedValues = fileExistsFactory.getSelectors()}};
        DefaultParameter<std::string> renamePattern = {{.longKey      = "rename-pattern",
                                                        .argumentName = "PATTERN",
                                                        .doc          = "File rename pattern. Defaults to \" ({})\"",
                                                        .defaultValue = " ({})"}};
        DefaultParameter<bool> recursive            = {
                {.shortKey = 'r', .longKey = "recursive", .doc = "Sort directories recursively", .defaultValue = false}};
    };

    const auto& parameters = CommandLineParser::defaultParse<FileSorterParameters>(argc, argv);
    auto fetcher           = FileListFetcher(bsc::fetchers::filesystemFileListFetcher,FetcherConfig{.recursive=parameters.recursive()},{});
    FileSorter fileSorter(fetcher,
                          {.sortStrategy                  = actionFactory.create(parameters.action(), {}),
                           .createValidTargetPathStrategy = fileExistsFactory.create(parameters.fileExists(), {parameters.renamePattern()}),
                           .errorHandlerStrategy          = errorActionFactory.create(parameters.errorHandler(), {}),
                           .fileExistsPredicate           = StandardFileSorterPredicates::fileExistsPredicate});
    for (MimeFileTypeFactory factory; const auto& [mime, pattern] : parameters.mimeMatchers()) {
        fileSorter.addPattern(std::make_unique<FileSorterMimeMatcher>(factory.create(mime)), pattern);
    }
    for (const auto& [name, pattern] : parameters.nameMatchers()) {
        fileSorter.addPattern(std::make_unique<FileSorterNameMatcher>(name), pattern);
    }
    fileSorter.sort(parameters.targetPaths()) | StandardResultConsumers::printResult;
    return 0;
}