//
// Created by Krzysztof Tulidowicz on 22.04.2020.
//
#include "../../../optional/optional/optional.h"
#include "file-sorter/properties/Actions.h"
#include "file-sorter/properties/Factories.h"
#include "file-sorter/properties/FileSorterMapperProperties.h"
#include "file-sorter/properties/FileSorterPatternFactory.h"
#include <core/factory/FactoryParameter.h>
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
    FactoryParameter<FileSortingStrategies::SortStrategy> action                = {{.shortKey     = 'a',
                                                                                    .longKey      = "action",
                                                                                    .argumentName = "ACTION",
                                                                                    .doc          = "Action to perform on files\n"
                                                                                                    "Available actions:\ncopy *\nmove\npretend",
                                                                                    .defaultSelector = SortAction::copy}};
    FactoryParameter<FileSortingStrategies::ErrorHandlingStrategy> errorHandler = {{.shortKey     = 'e',
                                                                                    .longKey      = "error",
                                                                                    .argumentName = "ERROR HANDLER",
                                                                                    .doc          = "What to do when error occurs.\n"
                                                                                                    "Available handlers:\nstop *\nignore",
                                                                                    .defaultSelector = "stop"}};
    FactoryParameter<FileSortingStrategies::RelativePathBuilder> relativePath   = {
            {.shortKey     = 'R',
               .longKey      = "relative",
               .argumentName = "RELATIVE PATH BUILDER",
               .doc          = "Whether relative path should be preserved on target.\n"
                               "Available builders:\n"
                               "none *\n"
                               "parent\n"
                               "preserve",
               .defaultSelector = "none"}};
    FactoryParameter<FileSortingStrategies::CreateValidTargetPathStrategy> fileExists = {
            {.shortKey        = 'f',
             .longKey         = "file-exists",
             .argumentName    = "ACTION",
             .doc             = "What to do when target file exists.\n"
                                "Available actions:\n"
                                "rename *\nskip\noverwrite\nabort",
             .defaultSelector = "rename",
             .factoryDetails  = {.argumentFetcher = {[this]() { return std::vector{this->renamePattern()}; }

                                }}}};
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
    DefaultParameter<bool> verbose = {{.shortKey = 'v', .longKey = "verbose", .defaultValue = false}};
};

void addOrUpdateRules(FileSorterProperties& properties,
                      const std::map<std::string, std::string>& matchers,
                      MapperType type,
                      MapperMatcherMode mode,
                      const FileSorterParameters& parameters);

int main(int argc, char* argv[]) {
    auto context = Context::makeContext();
    Context::setActiveContext(context);
    auto actionFactory       = makeActionFactory();
    auto errorActionFactory  = makeErrorActionFactory();
    auto fileExistsFactory   = makeFileExistsFactory();
    auto relativePathFactory = makeRelativePathFactory();

    const auto& parameters   = CommandLineParser::defaultParse<FileSorterParameters>(argc, argv);

    bsc::PropertyFileLoader propertyLoader(parameters.config() | optional::orElse(fs::path{}),
                                           {PropertySetting::ignoreMissingFile, PropertySetting::ignoreMissingProperty});

    auto fetcher = FileListFetcher(bsc::fetchers::filesystemFileListFetcher, FetcherConfig{.recursive = parameters.recursive()}, {});
    FileSorter fileSorter{
            fetcher,
            {.fileExistsPredicate = StandardFileSorterPredicates::fileExistsPredicate, .relativePathBuilder = *parameters.relativePath()}};

    FileSorterProperties fileSorterProperties{};

    addOrUpdateRules(fileSorterProperties, parameters.mimeMatchers(), MapperType::mime, MapperMatcherMode::none, parameters);
    addOrUpdateRules(fileSorterProperties, parameters.nameMatchers(), MapperType::regex, MapperMatcherMode::none, parameters);
    addOrUpdateRules(fileSorterProperties, parameters.sizeLessMatchers(), MapperType::size, MapperMatcherMode::less, parameters);
    addOrUpdateRules(fileSorterProperties, parameters.sizeGreaterMatchers(), MapperType::size, MapperMatcherMode::greater, parameters);

    for (FileSorterPatternFactory factory{}; const auto& property : fileSorterProperties.rules()) {
        fileSorter.addPattern(factory.createPatternMatchers(property),
                              property.pattern(),
                              FileSortingStrategies(actionFactory->create(property.actions().sort()),
                                                    fileExistsFactory->create(property.actions().fileExists().action(),
                                                                              {property.actions().fileExists().pattern()}),
                                                    errorActionFactory->create(property.actions().error())));
    }

    parameters.saveConfig() | optional::ifPresent([&fileSorterProperties](auto& savePath) {
        std::ofstream out(savePath);
        YamlWriter writer;
        writer << fileSorterProperties;
        out << writer << std::endl;
    });

    fileSorter.sort(parameters.targetPaths()) | StandardResultConsumers::printResult;
    return 0;
}

void addOrUpdateRules(FileSorterProperties& properties,
                      const std::map<std::string, std::string>& matchers,
                      MapperType type,
                      MapperMatcherMode mode,
                      const FileSorterParameters& parameters) {

    for (const auto& [matcher, pattern] : matchers) {
        properties.addOrUpdateRule(type,
                                   matcher,
                                   pattern,
                                   mode,
                                   parameters.action.getSelector() | optional::orElseThrow(std::invalid_argument("no action selector")),
                                   parameters.errorHandler.getSelector() | optional::orElseThrow(std::invalid_argument("no error selector")),
                                   parameters.fileExists.getSelector() | optional::orElseThrow(std::invalid_argument("no file exist selector")),
                                   parameters.renamePattern());
    }
}
