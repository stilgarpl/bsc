#include <utility>

#include <utility>

//
// Created by stilg on 15.09.2017.
//

#include <core/log/Logger.h>
#include "Context.h"


thread_local Context::Ptr Context::activeContext = nullptr;

Context::Ptr Context::getParentContext() const {
    return parentContext;
}

void Context::setParentContext(Context::Ptr parentContext) {
    std::lock_guard<std::recursive_mutex> guard(contextLock);
    //@todo maybe we need to make sure here that we are not making a loop and throw exeption if we are?
    Context::parentContext = std::move(parentContext);
}

Context::Ptr Context::getActiveContext() {
    if (hasActiveContext()) {
        return activeContext;
    } else {
        ERROR("No active context")
        throw InvalidContextException("No active context");
    }
}

void Context::setActiveContext(Context::Ptr ctx) {
    activeContext = std::move(ctx);
}

Context::Context(const Context &other) {
    std::scoped_lock g(contextLock, other.contextLock);
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
    struct ContextMakeSharedWorkaround : public Context {
    };
    return std::make_shared<ContextMakeSharedWorkaround>();
}

bool Context::isDefaultContext() const {
    return defaultContext;
}

Context::Context(bool defaultContext) : defaultContext(defaultContext) {}

void Context::setDebugId(const std::string& debugId) {
    Context::debugId = debugId;
}

Context::~Context() {
//    LOGGER("context destructor " + debugId);

}

Context::ContextPtr Context::makeContext(const Context::Ptr& parentContext) {
    struct ContextMakeSharedWorkaround : public Context {
        explicit ContextMakeSharedWorkaround(const Context& other) : Context(other) {}
    };
    if (parentContext != nullptr) {
        //this was using copy constructor and needlessly copying everything in parent context to this new context.
        ContextPtr ret = makeContext();//std::make_shared<ContextMakeSharedWorkaround>(*parentContext);
        ret->setParentContext(parentContext);
        return ret;
    } else {
        LOGGER("error: NULL CONTEXT PASSED")
        throw InvalidContextException("Null context passed");
    }
}

bool Context::hasActiveContext() {
    return activeContext != nullptr;
}

InvalidContextException::InvalidContextException(const std::string& arg) : invalid_argument(arg) {}

InvalidContextException::InvalidContextException(const char* string) : invalid_argument(string) {}
