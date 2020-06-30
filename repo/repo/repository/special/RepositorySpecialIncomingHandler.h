//
// Created by stilgar on 09.04.2020.
//

#ifndef BSC_REPOSITORYSPECIALINCOMINGHANDLER_H
#define BSC_REPOSITORYSPECIALINCOMINGHANDLER_H

#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;
    //@todo remove this class
    class RepositorySpecialIncomingHandler {
        const fs::path &incomingPath;

    public:
        RepositorySpecialIncomingHandler(const fs::path &incomingPath);

        void handleIncoming(const fs::path &path);

        void processIncomingFile(const fs::path &path);
    };

    class RepositoryIncomingHandlerException : public std::domain_error {
    public:
        RepositoryIncomingHandlerException(const std::string &arg);
    };
}// namespace bsc

#endif