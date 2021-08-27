//
// Created by Krzysztof Tulidowicz on 27.08.2021.
//
#include "FileSorterMapperProperties.h"

#include <utility>

namespace bsc {
    void FileSorterProperties::write(PropertySequencer& sequencer) const {
        sequencer(version,rules);
    }
    void FileSorterProperties::addOrUpdateRule(MapperType mapper,
                                               std::string match,
                                               std::string pattern,
                                               SortAction sort,
                                               std::string errorAction,
                                               std::string fileExists) {

        auto result = std::ranges::find_if(this->rules(),[&mapper, &match](auto& i) {
            return i.type() == mapper && i.match() == match;
        });
        FileSorterMapperProperties* ptr;
        if (result != std::end(this->rules())) {
            ptr = &*result;
        } else {
            ptr = &this->rules().emplace_back();
            ptr->type = mapper;
            ptr->match = match;
        }
        ptr->pattern = std::move(pattern);
        ptr->actions().sort = sort;
        ptr->actions().fileExists = std::move(fileExists);
        ptr->actions().error = std::move(errorAction);
    }
    void FileSorterMapperProperties::write(PropertySequencer& sequencer) const {
        sequencer(type,match,pattern,actions);
    }
    void ActionProperties::write(PropertySequencer& sequencer) const {
        sequencer(sort,error,fileExists);
    }
}
