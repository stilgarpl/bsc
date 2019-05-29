#include <utility>

#include <utility>

//
// Created by stilg on 15.09.2017.
//

#include <p2p/log/Logger.h>
#include "Context.h"


thread_local Context::Ptr Context::activeContext = nullptr;

Context::Ptr Context::getParentContext() const {
    return parentContext;
}

void Context::setParentContext(Context::Ptr parentContext) {
    std::lock_guard<std::recursive_mutex> guard(contextLock);
    Context::parentContext = std::move(parentContext);
}

Context::Ptr Context::getActiveContext() {
    thread_local static Context::Ptr defaultContext = std::make_shared<Context>(true);
//    if (activeContext == nullptr) {
//        LOGGER("WARNING: returning default context")
//        return defaultContext;
//    } else {
        return activeContext;
//    }
}

void Context::setActiveContext(Context::Ptr ctx) {
    activeContext = std::move(ctx);
}

Context::Context(const Context &other) {
    for (auto &&item : other.data) {
        this->data[item.first] = item.second;
    }
    this->parentContext = other.parentContext;
}

//Context &Context::operator+=(const Context::Ptr other) {
//    std::lock_guard<std::recursive_mutex> guard(contextLock);
//    for (auto &&item : other->data) {
//        // std::clog << "Context::+= copying id " << item.first << std::endl;
//        this->data[item.first] = item.second;
//    }
//
//    return *this;
//}

Context &Context::operator+=(const Context::Ptr &other) {
    std::lock_guard<std::recursive_mutex> guard(contextLock);

    for (auto &&item : other->data) {
        //std::clog << "Context::+= copying id " << item.first << std::endl;
        this->data[item.first] = item.second;
    }

    setParentContext(other);
    return *this;
}

Context::Context(const Context::Ptr &ptr) : Context(*ptr) {}

Context::ContextPtr Context::makeContext() {
    return std::make_shared<Context>();
}

bool Context::isDefaultContext() const {
    return defaultContext;
}

Context::Context(bool defaultContext) : defaultContext(defaultContext) {}

void Context::setDebug_id(const std::string &debug_id) {
    Context::debug_id = debug_id;
}

Context::~Context() {
//    LOGGER("context destructor " + debug_id);

}
