//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTYCONTEXT_H
#define BSC_PROPERTYCONTEXT_H

#include "PropertySetting.h"
#include <context/context/AutoContextSetter.h>
#include <context/options/Flags.h>
#include <properties/control/parser/PropertyParser.h>
#include <properties/control/writer/PropertyWriter.h>
#include <utility>
namespace bsc {

    class PropertiesNotLoaded : public std::domain_error {
    public:
        explicit PropertiesNotLoaded(const std::string& arg);
    };

    class PropertyContext final : public AutoContextSetter<PropertyContext> {
    private:
        std::recursive_mutex mutex;
        std::unique_ptr<PropertyParser> propertyParser = nullptr;
        std::unique_ptr<PropertyWriter> propertyWriter = nullptr;
        Flags<PropertySetting> propertySettings;
    public:
        PropertyParser& getPropertyParser() {
            if (propertyParser == nullptr) {
                throw PropertiesNotLoaded("Property parser is not set");
            }
            return *propertyParser;
        }

         [[nodiscard]] const auto& getPropertyConfiguration() const {
             return propertySettings;
         }

        template<IsPropertyParser ParserType, typename... Args>
        void setPropertyParser(Args&&... args) {
            propertyParser = std::make_unique<ParserType>(std::forward<Args...>(args...));
        }

        bool hasPropertyParser() { return propertyParser != nullptr; }

        void removePropertyParser() { propertyParser = nullptr; }

        PropertyWriter& getPropertyWriter() {
            if (propertyWriter == nullptr) {
                throw PropertiesNotLoaded("Property writer is not set");
            }
            return *propertyWriter;
        }

        template<IsPropertyWriter WriterType, typename... Args>
        void setPropertyWriter(Args&&... args) {
            propertyWriter = std::make_unique<WriterType>(std::forward<Args...>(args...));
        }

        bool hasPropertyWriter() { return propertyWriter != nullptr; }

        void removePropertyWriter() { propertyWriter = nullptr; }



        void enableConfigurations(std::initializer_list<PropertySetting> settings) {
            propertySettings.enable(settings);
        }

        void disableConfigurations(std::initializer_list<PropertySetting> settings) {
            propertySettings.disable(settings);
        }
        friend class PropertyController;


        void push() {
            if (hasPropertyParser()) {
                propertyParser->push();
            }
            if (hasPropertyWriter()) {
                propertyWriter->push();
            }
        }

        void pop() {
            if (hasPropertyParser()) {
                propertyParser->pop();
            }
            if (hasPropertyWriter()) {
                propertyWriter->pop();
            }
        }
    };
}// namespace bsc

#endif