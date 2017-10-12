//
// Created by stilg on 15.09.2017.
//

#include "Context.h"


thread_local Context* Context::activeContext = nullptr;

Context *Context::getParentContext() const {
    return parentContext;
}

void Context::setParentContext(Context *parentContext) {
    Context::parentContext = parentContext;
}

Context &Context::getActiveContext() {
    thread_local static Context defaultContext;
    if (activeContext == nullptr) {
        return defaultContext;
    } else {
        return *activeContext;
    }
}

void Context::setActiveContext(Context *ctx) {
    activeContext = ctx;
}

Context::Context(const Context &other) {
    for (auto &&item : other.data) {
        this->data[item.first] = item.second;
    }
}

Context &Context::operator+=(const Context &other) {
    for (auto &&item : other.data) {
        std::clog << "Context::+= copying id " << item.first << std::endl;
        this->data[item.first] = item.second;
    }

    return *this;
}

Context &Context::operator+=(Context &other) {
    for (auto &&item : other.data) {
        std::clog << "Context::+= copying id " << item.first << std::endl;
        this->data[item.first] = item.second;
    }

    //@todo think about this:
    setParentContext(&other);
    return *this;
}
