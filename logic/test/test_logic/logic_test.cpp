//
// Created by stilgar on 9/27/19.
//

#include <catch2/catch.hpp>
#include <atomic>
#include <logic/SourceManager.h>
#include <logic/IEvent.h>
#include <logic/sources/EventQueueSource.h>
#include <logic/LogicObject.h>

using namespace bsc;

class Ping : public bsc::IEvent<int> {

};

class PingSource : public bsc::EventQueueSource<Ping> {
public:
    explicit PingSource(SourceManager& sourceManager) : EventQueueSource(sourceManager) {}

    void ping() {
        auto event = newEvent();
        queueEvent(event);
    }
};


class SetupSimpleAction : public bsc::LogicObject {
    std::atomic_int counter = 0;
public:
    explicit SetupSimpleAction(bsc::LogicManager& logicManager) : LogicObject(logicManager) {
    }

    void setupActions(SetupActionHelper& actionHelper) override {
        when(event<Ping>()).fireNewAction([&](auto event) { counter++; });
    }

    bool assignActions(AssignActionHelper& actionHelper) override {
        return true;
    }

    bool setupSources(SetupSourceHelper &sourceHelper) override {
        sourceHelper.requireSource<PingSource>();
        return true;
    }

    auto &getCounter() { return counter; };

};

class SetupChainAction : public bsc::LogicObject {
    std::atomic_int firstCounter = 0;
    std::atomic_int secondCounter = 0;
public:
    explicit SetupChainAction(bsc::LogicManager& logicManager) : LogicObject(logicManager) {
    }

    void setupActions(SetupActionHelper& actionHelper) override {
        when(event<Ping>())
                .newChain("chain")
                .fireNewChainAction([&](auto event) {
                    firstCounter++;
                    return event;
                })
                .fireNewChainAction([&](auto event) {
                    secondCounter++;
                    return event;
                });
    }

    bool assignActions(AssignActionHelper& actionHelper) override {
        return true;
    }

    bool setupSources(SetupSourceHelper& sourceHelper) override {
        sourceHelper.requireSource<PingSource>();
        return true;
    }

    auto& getFirstCounter() { return firstCounter; };

    auto& getSecondCounter() { return secondCounter; };

};


class SetupInvalidChainAction : public bsc::LogicObject {
    std::atomic_int firstCounter = 0;
    std::atomic_int secondCounter = 0;
public:
    explicit SetupInvalidChainAction(bsc::LogicManager& logicManager) : LogicObject(logicManager) {
    }

    void setupActions(SetupActionHelper& actionHelper) override {
        when(event<Ping>())
//                .newChain("chain") this is invalid, no chain id
                .fireNewChainAction([&](auto event) {
                    firstCounter++;
                    return event;
                })
                .fireNewChainAction([&](auto event) {
                    secondCounter++;
                    return event;
                });
    }

    bool assignActions(AssignActionHelper& actionHelper) override {
        return true;
    }

    bool setupSources(SetupSourceHelper& sourceHelper) override {
        sourceHelper.requireSource<PingSource>();
        return true;
    }

    auto& getFirstCounter() { return firstCounter; };

    auto& getSecondCounter() { return secondCounter; };

};

class SetupInvalidAssignment : public bsc::LogicObject {
public:
    SetupInvalidAssignment(bsc::LogicManager& logicManager) : LogicObject(logicManager) {}

public:
    void setupActions(SetupActionHelper& actionHelper) override {
        when(event<Ping>()).fireAction("wrong action ID");
    }

    bool assignActions(AssignActionHelper& actionHelper) override {
        return true;
    }

    bool setupSources(SetupSourceHelper& sourceHelper) override {
        return true;
    }

};

void waitFor(const std::function<bool(void)>& expression, std::chrono::milliseconds timeout) {

    auto beginTime = std::chrono::steady_clock::now();
    while (!expression() || std::chrono::steady_clock::now() - beginTime < timeout) {
        std::this_thread::sleep_for(10ms);
    }
}


TEST_CASE("Basic logic test") {

//before
    bsc::Context::OwnPtr context = bsc::Context::makeContext();
    bsc::Context::setActiveContext(context);
    bsc::LogicManager logicManager;
    logicManager.setContexts(context);
//@todo think about it, doesn't it seem backwards? maybe logicManager should call setupLogic on all of its logic objects? or maybe not. Node calls setup logic on its modules in specific moment
    SetupSimpleAction setupLogic(logicManager);
    setupLogic.setupLogic();
    setupLogic.getCounter().store(0);
//
//when
    logicManager.start();
    auto pingSource = logicManager.getSource<PingSource>();
    for (int j = 0; j < 1000; ++j) {
        pingSource->ping();
    }
    waitFor([&] { return setupLogic.getCounter().load() == 1000; }, 100ms);
    logicManager.stop();
    REQUIRE(setupLogic.getCounter().load() == 1000);
    LOGGER("all ok")
    logicManager.join();


}


TEST_CASE("Chain logic test") {
    const int count = 1000;
//before
    bsc::Context::OwnPtr context = bsc::Context::makeContext();
    bsc::Context::setActiveContext(context);
    bsc::LogicManager logicManager;
    logicManager.setContexts(context);
//@todo think about it, doesn't it seem backwards? maybe logicManager should call setupLogic on all of its logic objects? or maybe not. Node calls setup logic on its modules in specific moment. But it would be better. Right now setupLogic() can be called many times, breaking logic.
    SetupChainAction setupLogic(logicManager);
    setupLogic.setupLogic();
    setupLogic.getFirstCounter().store(0);
    setupLogic.getSecondCounter().store(0);
//
//when
    logicManager.start();
    auto pingSource = logicManager.getSource<PingSource>();
    for (int j = 0; j < count; ++j) {
        pingSource->ping();
    }
    waitFor([&] { return setupLogic.getSecondCounter().load() == count; }, 100ms);
    logicManager.stop();
    REQUIRE(setupLogic.getFirstCounter().load() == count);
    REQUIRE(setupLogic.getSecondCounter().load() == count);
    LOGGER("all ok")
    logicManager.join();


}

TEST_CASE("Invalid chain logic test") {
    bsc::Context::OwnPtr context = bsc::Context::makeContext();
    bsc::Context::setActiveContext(context);
    bsc::LogicManager logicManager;
    logicManager.setContexts(context);
    SetupInvalidChainAction setupLogic(logicManager);
    REQUIRE_THROWS_AS(setupLogic.setupLogic(), bsc::InvalidChainException);
}


TEST_CASE("Logic exceptions test") {
    bsc::Context::OwnPtr context = bsc::Context::makeContext();
    bsc::Context::setActiveContext(context);
    bsc::LogicManager logicManager;
    logicManager.setContexts(context);
    SetupInvalidAssignment setupLogic(logicManager);
    REQUIRE_THROWS_AS(setupLogic.setupLogic(), bsc::LogicAssignmentException);
}