//
// Created by stilgar on 05.12.2019.
//

#ifndef BASYCO_INPUTOUTPUTCONTEXT_H
#define BASYCO_INPUTOUTPUTCONTEXT_H


#include <iosfwd>

class InputOutputContext {

public:
    virtual std::ostream& out() = 0;

    virtual std::istream& in() = 0;

    static InputOutputContext& active();

};

class StandardInputOutputContext : public InputOutputContext {
public:
    std::ostream& out() override;

    std::istream& in() override;
};


#endif //BASYCO_INPUTOUTPUTCONTEXT_H
