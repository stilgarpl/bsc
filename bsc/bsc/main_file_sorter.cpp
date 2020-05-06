//
// Created by stilgar on 22.04.2020.
//
#include <parser/parameters/CommandLineParameters.h>
#include <string>

using namespace bsc;
using namespace std::string_literals;
struct FileSorterParameters : CommandLineParameters {
    Usage usage = {"File sorter utility"s};
};
int main(int argc, char* argv[]) {

    const auto& parameters = CommandLineParameters::parse<FileSorterParameters>(argc, argv);
    //@todo implement file sorter exec
    return 0;
}