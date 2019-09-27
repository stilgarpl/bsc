//
// Created by stilgar on 9/27/19.
//

#include <catch2/catch.hpp>
#include <atomic>
#include <logic/SourceManager.h>
#include <logic/IEvent.h>
#include <logic/sources/EventQueueSource.h>
#include <logic/LogicObject.h>


class Ping : public IEvent<int> {

};

class PingSource : public EventQueueSource<Ping> {
public:
    explicit PingSource(SourceManager &sourceManager) : EventQueueSource(sourceManager) {}

    void ping() {
        auto event = newEvent();
        queueEvent(event);
    }
};


class SetupSimpleAction : public LogicObject {
    std::atomic_int counter;
public:
    explicit SetupSimpleAction(LogicManager &logicManager) : LogicObject(logicManager) {
        counter.store(0);
    }

    void setupActions(SetupActionHelper &actionHelper) override {
        when(event<Ping>()).fireNewAction([&](auto event) { counter++; });
    }

    bool assignActions(AssignActionHelper &actionHelper) override {
        return true;
    }

    bool setupSources(SetupSourceHelper &sourceHelper) override {
        sourceHelper.requireSource<PingSource>();
        return true;
    }

    auto &getCounter() { return counter; };

};

class SetupChainAction : public LogicObject {
    std::atomic_int firstCounter;
    std::atomic_int secondCounter;
public:
    explicit SetupChainAction(LogicManager &logicManager) : LogicObject(logicManager) {
        firstCounter.store(0);
        secondCounter.store(0);
    }

    void setupActions(SetupActionHelper &actionHelper) override {
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

    bool assignActions(AssignActionHelper &actionHelper) override {
        return true;
    }

    bool setupSources(SetupSourceHelper &sourceHelper) override {
        sourceHelper.requireSource<PingSource>();
        return true;
    }

    auto &getFirstCounter() { return firstCounter; };

    auto &getSecondCounter() { return secondCounter; };

};

void waitFor(const std::function<bool(void)> &expression, std::chrono::milliseconds timeout) {

    auto beginTime = std::chrono::steady_clock::now();
    while (!expression() || std::chrono::steady_clock::now() - beginTime < timeout) {
        std::this_thread::sleep_for(10ms);
    }
}


TEST_CASE("Basic logic test") {

//before
    Context::OwnPtr context = Context::makeContext();
    Context::setActiveContext(context);
    LogicManager logicManager;
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
    logicManager.stop();
    waitFor([&] { return setupLogic.getCounter().load() == 1000; }, 100ms);
    REQUIRE(setupLogic.getCounter().load() == 1000);
    LOGGER("all ok")
    logicManager.join();


}


TEST_CASE("Chain logic test") {
    const int COUNT = 1000;
//before
    Context::OwnPtr context = Context::makeContext();
    Context::setActiveContext(context);
    LogicManager logicManager;
    logicManager.setContexts(context);
//@todo think about it, doesn't it seem backwards? maybe logicManager should call setupLogic on all of its logic objects? or maybe not. Node calls setup logic on its modules in specific moment
    SetupChainAction setupLogic(logicManager);
    setupLogic.setupLogic();
    setupLogic.getFirstCounter().store(0);
    setupLogic.getSecondCounter().store(0);
//
//when
    logicManager.start();
    auto pingSource = logicManager.getSource<PingSource>();
    for (int j = 0; j < COUNT; ++j) {
        pingSource->ping();
    }
    logicManager.stop();
    waitFor([&] { return setupLogic.getSecondCounter().load() == COUNT; }, 100ms);
    REQUIRE(setupLogic.getFirstCounter().load() == COUNT);
    REQUIRE(setupLogic.getSecondCounter().load() == COUNT);
    LOGGER("all ok")
    logicManager.join();


}