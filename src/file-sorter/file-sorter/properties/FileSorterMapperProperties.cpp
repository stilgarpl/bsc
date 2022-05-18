//
// Created by Krzysztof Tulidowicz on 27.08.2021.
//
#include "FileSorterMapperProperties.h"

#include <utility>

namespace bsc {
    void FileSorterProperties::write(PropertySequencer& sequencer) const {
        sequencer(version, rules);
    }
    FileSorterMapperProperties& FileSorterProperties::addOrUpdateRule(MapperType mapperType,
                                                                      std::string match,
                                                                      std::string pattern,
                                                                      SortAction sort,
                                                                      std::string errorAction,
                                                                      std::string fileExists,
                                                                      std::string renamePattern) {

        auto found = std::ranges::find_if(this->rules(), [&pattern](FileSorterMapperProperties& j) { return j.pattern() == pattern; });
        FileSorterMapperProperties& ptr = found != std::end(this->rules())?
                                                                           *std::to_address(found) :
                                                                           this->rules().emplace_back();

        //@todo think about merging same matchers.
        auto& matcher                       = ptr.matchers().emplace_back();
        matcher.type                        = mapperType;
        matcher.match                       = std::move(match);

        ptr.pattern                        = std::move(pattern);
        ptr.actions().sort                 = sort;
        ptr.actions().fileExists().action  = std::move(fileExists);
        ptr.actions().fileExists().pattern = std::move(renamePattern);
        ptr.actions().error                = std::move(errorAction);
        return ptr;
    }
    void FileSorterMapperProperties::write(PropertySequencer& sequencer) const {
        sequencer(matchers, pattern, actions);
    }
    void ActionProperties::write(PropertySequencer& sequencer) const {
        sequencer(sort, error, fileExists);
    }
    void MatcherProperties::write(PropertySequencer& sequencer) const {
        sequencer(type, match);
    }
    void FileExistsProperties::write(PropertySequencer& sequencer) const {
        sequencer(action, pattern);
    }
}// namespace bsc
