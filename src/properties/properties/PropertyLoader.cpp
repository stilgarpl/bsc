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
        } else {
            context = Context::getActiveContext();
        }
        if (Context::getActiveContext()->get<PropertyContext>()->hasPropertyParser()) {
            throw PropertyContextAlreadyLoaded("Property context already loaded!");
        }
    }
    void PropertyLoader::enableOptions(std::initializer_list<PropertySetting> settings) {
        context->get<PropertyContext>()->enableConfigurations(settings);
    }
    void PropertyLoader::disableOptions(std::initializer_list<PropertySetting> settings) {
        context->get<PropertyContext>()->enableConfigurations(settings);
    }
}// namespace bsc
