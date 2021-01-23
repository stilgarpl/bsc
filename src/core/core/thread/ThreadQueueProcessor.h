//
// Created by Krzysztof Tulidowicz on 14.10.2019.
//

#ifndef BSC_THREADQUEUEPROCESSOR_H
#define BSC_THREADQUEUEPROCESSOR_H

#include <atomic>
#include <condition_variable>
#include <context/context/Context.h>
#include <core/log/Logger.h>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

using namespace std::chrono_literals;

namespace bsc {
    template<typename TokenType, typename SenderType>
    class ThreadQueueProcessor {
    public :
        using Token = TokenType;
        using SenderTypePtr = std::reference_wrapper<SenderType>;
        using UpdateFuncType = std::function<void(TokenType, SenderType&)>;
    private:
        std::queue<std::pair<TokenType, SenderTypePtr>> queue;
        std::condition_variable queueReady;
        std::mutex queueLock;
        std::unique_ptr<std::thread> processorThread;
        std::atomic_bool running;
        UpdateFuncType updateFunc;

    public:

        void process(TokenType token, SenderTypePtr senderPtr) {
            auto pair = std::make_pair(token, senderPtr);
            {
                std::unique_lock lock(queueLock);
                queue.push(pair);
            }
            queueReady.notify_all();
        }

    protected:

        void work(Context::Ptr contextPtr) {
            Context::setActiveContext(std::move(contextPtr));
            while (running) {
                std::unique_lock g(queueLock);
                //@todo 1s or other value?
                queueReady.wait_for(g, 1s, [&]() -> bool { return (!queue.empty() || !running); });

                while (!queue.empty()) {
                    auto&[token, sender] = queue.front();
                    g.unlock();
                    updateFunc(token, sender);
                    g.lock();
                    queue.pop();
                }
            }
        }

    public:
        explicit ThreadQueueProcessor(UpdateFuncType update) {
            running = true;
            updateFunc = update;
            processorThread = std::make_unique<std::thread>(&ThreadQueueProcessor::work, this,
                                                            Context::getActiveContext());
        }


    public:
        virtual ~ThreadQueueProcessor() {
            running = false;
            queueReady.notify_all();
            processorThread->join();
            LOGGER("thread queue processor finished")

        }
    };
}


#endif //BSC_THREADQUEUEPROCESSOR_H
