//
// Created by Krzysztof Tulidowicz on 22.04.2020.
//
#include <filesystem>
#include <fstream>
#include <io/sorter/FileSorter.h>
#include <io/sorter/fetchers/FileListFetcher.h>
#include <io/sorter/fetchers/FilesystemFileListFetcher.h>
#include <io/sorter/mappers/FileSorterMimeMatcher.h>
#include <io/sorter/mappers/FileSorterNameMatcher.h>
#include <io/sorter/strategies/StandardFileSorterStrategies.h>
#include <parser/parameters/CommandLineParameters.h>
#include <properties/PropertyFileLoader.h>
#include <properties/control/writer/YamlWriter.h>
#include <string>
using namespace bsc;
using namespace std::string_literals;

#include "file-sorter/properties/Actions.h"
#include "file-sorter/properties/Factories.h"
#include "file-sorter/properties/FileSorterMapperProperties.h"
#include "file-sorter/properties/FileSorterPatternFactory.h"

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
        DefaultParameter<std::map<std::string, std::string>> sizeLessMatchers = {
                {.shortKey = 's', .longKey = "size", .argumentName = "size=PATTERN", .doc = "Pair of size (less than) and path pattern"}};
        DefaultParameter<std::map<std::string, std::string>> sizeGreaterMatchers = {
                {.shortKey = 'S', .longKey = "SIZE", .argumentName = "size=PATTERN", .doc = "Pair of size (greater than) and path pattern"}};
        DefaultParameter<SortAction> action        = {{.shortKey     = 'a',
                                                       .longKey      = "action",
                                                       .argumentName = "ACTION",
                                                       .doc          = "Action to perform on files\nAvailable actions:\ncopy *\nmove\npretend",
                                                       .defaultValue = SortAction::copy}};
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
        Parameter<fs::path> config = {
                {.shortKey = 'c', .longKey = "config", .argumentName = "PATH", .doc = "Configuration file location (load)"}};
        Parameter<fs::path> saveConfig = {
                {.shortKey = 'C', .longKey = "save", .argumentName = "PATH", .doc = "Configuration file location (save)"}};
    };

    const auto& parameters = CommandLineParser::defaultParse<FileSorterParameters>(argc, argv);

    fs::path configPath    = parameters.config().has_value() ? parameters.config().value() : "";
    bsc::PropertyFileLoader propertyLoader(configPath, {PropertySetting::ignoreMissingFile, PropertySetting::ignoreMissingProperty});

    FileSorterProperties fileSorterProperties{};

    auto fetcher = FileListFetcher(bsc::fetchers::filesystemFileListFetcher, FetcherConfig{.recursive = parameters.recursive()}, {});
    FileSorter fileSorter(fetcher,
                          {.sortStrategy                  = actionFactory.create(parameters.action()),
                           .createValidTargetPathStrategy = fileExistsFactory.create(parameters.fileExists(), {parameters.renamePattern()}),
                           .errorHandlerStrategy          = errorActionFactory.create(parameters.errorHandler()),
                           .fileExistsPredicate           = StandardFileSorterPredicates::fileExistsPredicate});

    for (const auto& [mime, pattern] : parameters.mimeMatchers()) {
        fileSorterProperties
                .addOrUpdateRule(MapperType::mime, mime, pattern, bsc::MapperMatcherMode::none, parameters.action(), parameters.errorHandler(), parameters.fileExists());
    }

    for (const auto& [name, pattern] : parameters.nameMatchers()) {
        fileSorterProperties
                .addOrUpdateRule(MapperType::regex, name, pattern, bsc::MapperMatcherMode::none,  parameters.action(), parameters.errorHandler(), parameters.fileExists());
    }

    for (const auto& [size, pattern] : parameters.sizeLessMatchers()) {
        fileSorterProperties
                .addOrUpdateRule(MapperType::size, size, pattern, bsc::MapperMatcherMode::less,  parameters.action(), parameters.errorHandler(), parameters.fileExists());
    }

    for (const auto& [size, pattern] : parameters.sizeGreaterMatchers()) {
        fileSorterProperties
                .addOrUpdateRule(MapperType::size, size, pattern, bsc::MapperMatcherMode::greater,  parameters.action(), parameters.errorHandler(), parameters.fileExists());
    }

    for (FileSorterPatternFactory factory{}; const auto& property : fileSorterProperties.rules()) {
        fileSorter.addPattern(factory.createPatternMatcher(property), property.pattern());
    }

    if (parameters.saveConfig()) {
        fs::path savePath = parameters.saveConfig().value();
        std::ofstream out(savePath);
        YamlWriter writer;
        writer << fileSorterProperties;
        out << writer << std::endl;
    }

    fileSorter.sort(parameters.targetPaths()) | StandardResultConsumers::printResult;
    return 0;
}