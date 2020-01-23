#include <utility>

#include <utility>

//
// Created by stilg on 15.09.2017.
//

#include <core/log/Logger.h>
#include "Context.h"


thread_local bsc::Context::Ptr bsc::Context::activeContext = nullptr;

bsc::Context::Ptr bsc::Context::getParentContext() const {
    return parentContext;
}

void bsc::Context::setParentContext(bsc::Context::Ptr parentContext) {
    std::lock_guard<std::recursive_mutex> guard(contextLock);
    Context::parentContext = std::move(parentContext);
    validateParentContext();
}

bsc::Context::Ptr bsc::Context::getActiveContext() {
    if (hasActiveContext()) {
        return activeContext;
    } else {
        ERROR("No active context")
        throw bsc::InvalidContextException("No active context");
    }
}

void bsc::Context::setActiveContext(Context::Ptr ctx) {
    activeContext = std::move(ctx);
}

bsc::Context::Context(const Context& other) {
    std::scoped_lock g(contextLock, other.contextLock);
    for (auto&& item : other.data) {
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

bsc::Context& bsc::Context::operator+=(const bsc::Context::Ptr& other) {
    std::lock_guard<std::recursive_mutex> guard(contextLock);

    for (auto&& item : other->data) {
        //std::clog << "Context::+= copying id " << item.first << std::endl;
        this->data[item.first] = item.second;
    }

    setParentContext(other);
    return *this;
}

bsc::Context::Context(const Context::Ptr& ptr) : Context(*ptr) {}

bsc::Context::ContextPtr bsc::Context::makeContext() {
    struct ContextMakeSharedWorkaround : public Context {
    };
    return std::make_shared<ContextMakeSharedWorkaround>();
}

bool bsc::Context::isDefaultContext() const {
    return defaultContext;
}

bsc::Context::Context(bool defaultContext) : defaultContext(defaultContext) {}

void bsc::Context::setDebugId(const std::string& debugId) {
    Context::debugId = debugId;
}

bsc::Context::~Context() {
//    LOGGER("context destructor " + debugId);

}

bsc::Context::ContextPtr bsc::Context::makeContext(const Context::Ptr& parentContext) {
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
        throw bsc::InvalidContextException("Null context passed");
    }
}

bool bsc::Context::hasActiveContext() {
    return activeContext != nullptr;
}

void bsc::Context::validateParentContext() {
    //@todo if performance becomes an issue, maybe remove this call in release version.
    // I don't think it's actually possible to make a context loop in my apps, but who knows what can happen.
    Context::Ptr ancestor = parentContext;
    while (ancestor != nullptr) {
        if (&*ancestor == this) {
            throw bsc::ContextLoopException("Context loop detected");
        } else {
            ancestor = ancestor->parentContext;
        }
    }
}

bsc::InvalidContextException::InvalidContextException(const std::string& arg) : invalid_argument(arg) {}

bsc::InvalidContextException::InvalidContextException(const char* string) : invalid_argument(string) {}

bsc::ContextLoopException::ContextLoopException(const std::string& arg) : domain_error(arg) {}

bsc::InvalidContextValueException::InvalidContextValueException(const std::string& arg) : invalid_argument(arg) {}
