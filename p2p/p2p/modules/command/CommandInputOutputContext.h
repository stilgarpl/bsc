//
// Created by stilgar on 06.12.2019.
//

#ifndef BSC_COMMANDINPUTOUTPUTCONTEXT_H
#define BSC_COMMANDINPUTOUTPUTCONTEXT_H


#include <core/io/InputOutputContext.h>
#include <sstream>


namespace bsc {
    class CommandInputOutputContext : public bsc::InputOutputContext {
    private:
        std::stringstream inputStream;
        std::stringstream outputStream;
        std::stringstream errorStream;
    public:
        std::ostream& out() override;

        std::ostream& err() override;

        std::istream& in() override;

        const std::stringstream& getInputStream() const;

        const std::stringstream& getErrorStream() const;

        const std::stringstream& getOutputStream() const;

    };
}


#endif //BSC_COMMANDINPUTOUTPUTCONTEXT_H
