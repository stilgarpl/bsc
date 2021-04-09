//
// Created by Krzysztof Tulidowicz on 06.01.2021.
//

#ifndef BSC_STATEMACHINEDEFINITION_H
#define BSC_STATEMACHINEDEFINITION_H

#include <map>
#include <set>
#include <stdexcept>

namespace bsc {

    template<typename StateIdType>
    class InvalidStateException : public std::domain_error {
    private:
        const StateIdType state;

    public:
        InvalidStateException(const StateIdType& state, const std::string& arg) : domain_error(arg), state(state) {}
        StateIdType getState() const { return state; }
    };

    template<typename StateIdType>
    class InvalidLinkException : public std::domain_error {
    private:
        const StateIdType stateFrom;
        const StateIdType stateTo;

    public:
        InvalidLinkException(const StateIdType& stateFrom, const StateIdType& stateTo, const std::string& arg)
            : domain_error(arg), stateFrom(stateFrom), stateTo(stateTo) {}
        StateIdType getStateFrom() const { return stateFrom; }
        StateIdType getStateTo() const { return stateTo; }
    };

    template<typename StateIdType>
    class StateMachineDefinition {
        std::set<StateIdType> states;
        std::map<StateIdType, std::set<StateIdType>> links;

    public:
        void addState(const StateIdType& state) {
            //        logger.debug("adding state" + std::to_string(state));
            states.insert(state);
        }

        template<typename... StateIdTypes>
        void addState(const StateIdTypes&... newStates) {
            StateIdType allStates[] = {newStates...};
            for (const auto& item : allStates) {
                addState(item);
            }
        }

        void addLink(const StateIdType& state1, const StateIdType& state2) {
            if (states.count(state1) > 0 && states.count(state2) > 0) {
                //            logger.debug("linking " + std::to_string(state1) + " to " + std::to_string(state2));
                links[state1].insert(state2);
            } else {
                throw InvalidLinkException(state1, state2, "Invalid Link");
            }
        }

        template<typename... StateIdTypes>
        void addLink(const StateIdType& state1, const StateIdTypes&... otherStates) {
            //@todo check if both states exist in states set
            StateIdType allStates[] = {otherStates...};
            for (const auto& item : allStates) {
                addLink(state1, item);
            }
        }

        bool hasState(const StateIdType& state) const { return states.find(state) != states.end(); }

        bool hasLink(const StateIdType& from, const StateIdType& to) const {
            return links.template contains(from) && links.at(from).template contains(to);
        }

        const StateIdType& getState(const StateIdType& state) const {
            auto tempState = states.find(state);
            if (tempState == states.end()) {
                throw InvalidStateException(state, "Invalid state");
            } else {
                return *tempState;
            }
        }
    };
}// namespace bsc

#endif