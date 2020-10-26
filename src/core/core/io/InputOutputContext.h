//
// Created by Krzysztof Tulidowicz on 05.12.2019.
//

#ifndef BSC_INPUTOUTPUTCONTEXT_H
#define BSC_INPUTOUTPUTCONTEXT_H


#include <iosfwd>

namespace bsc {
    class InputOutputContext {

    public:
        virtual std::ostream& out() = 0;

        virtual std::ostream& err() = 0;

        virtual std::istream& in() = 0;

        static InputOutputContext& active();

    };

    //@todo this should autoregister as InputOutputContext unless changed
    class StandardInputOutputContext : public bsc::InputOutputContext {
    public:
        std::ostream& out() override;

        std::ostream& err() override;

        std::istream& in() override;
    };
}


#endif //BSC_INPUTOUTPUTCONTEXT_H
