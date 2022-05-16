//
// Created by Krzysztof Tulidowicz on 27.08.2021.
//
#include "FileSorterMapperProperties.h"

#include <utility>

namespace bsc {
    void FileSorterProperties::write(PropertySequencer& sequencer) const {
        sequencer(version, rules);
    }
    FileSorterMapperProperties& FileSorterProperties::addOrUpdateRule(MapperType mapper,
                                                                      std::string match,
                                                                      std::string pattern,
                                                                      SortAction sort,
                                                                      std::string errorAction,
                                                                      std::string fileExists,
                                                                      std::string renamePattern) {

        auto found = std::ranges::find_if(this->rules(), [&mapper, &match](auto& j) {
            auto& matchers = j.matchers();
            return std::ranges::find_if(matchers, [&mapper, &match](auto& i) { return i.type() == mapper && i.match() == match; })
                           != std::end(matchers)
                   && matchers.size() == 1;
        });
        FileSorterMapperProperties* ptr;
        if (found != std::end(this->rules())) {
            ptr = &*found;
        } else {
            ptr           = &this->rules().emplace_back();
            auto& matcher = ptr->matchers().emplace_back();
            matcher.type  = mapper;
            matcher.match = std::move(match);
        }
        ptr->pattern              = std::move(pattern);
        ptr->actions().sort       = sort;
        ptr->actions().fileExists().action = std::move(fileExists);
        ptr->actions().fileExists().pattern = std::move(renamePattern);
        ptr->actions().error      = std::move(errorAction);
        return *ptr;
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
