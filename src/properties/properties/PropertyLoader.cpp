//
// Created by Krzysztof Tulidowicz on 29.01.2021.
//

#include "PropertyLoader.h"
#include "PropertyContext.h"
#include "PropertyExceptions.h"

namespace bsc {
    PropertyLoader::~PropertyLoader() { Context::getActiveContext()->get<PropertyContext>()->removePropertyParser(); }
    PropertyLoader::PropertyLoader() {
        if (!Context::hasActiveContext() || !Context::getActiveContext()->has<PropertyContext>()) {
            Context::OwnPtr ownContext = Context::makeContext(true);
            context                    = ownContext;
            Context::setActiveContext(context);
        }
        if (Context::getActiveContext()->get<PropertyContext>()->hasPropertyParser()) {
            throw PropertyContextAlreadyLoaded("Property context already loaded!");
        }
    }
}// namespace bsc
