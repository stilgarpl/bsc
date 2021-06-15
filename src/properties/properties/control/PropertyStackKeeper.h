//
// Created by stilgar on 14.06.2021.
//

#ifndef BSC_PROPERTYSTACKKEEPER_H
#define BSC_PROPERTYSTACKKEEPER_H

namespace bsc{
    template<typename T>
    concept PropertyStack = requires(T& t) {
        t.push();
        t.pop();
    };

    template<PropertyStack StackType>
    class PropertyStackKeeper {
        StackType& propertyStack;

    public:
        explicit PropertyStackKeeper(StackType& stack) : propertyStack(stack) {
            propertyStack.push(); };
        virtual ~PropertyStackKeeper() { propertyStack.pop(); };
    };
}


#endif// BSC_PROPERTYSTACKKEEPER_H
