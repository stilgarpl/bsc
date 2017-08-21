//
// Created by stilgar on 21.08.17.
//

#include "ListContextSetup.h"

void ListContextSetup::setup(Context &context) {

    for (auto i: setupList) {
        i->setup(context);
    }

}

void ListContextSetup::addSetup(std::shared_ptr<IContextSetup> setup) {
    setupList.push_back(setup);

}
