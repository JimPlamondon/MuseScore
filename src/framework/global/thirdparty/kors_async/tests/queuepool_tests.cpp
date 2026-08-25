/*
MIT License

Copyright (c) 2025 Igor Korsukov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <mutex>
#include <thread>
#include <chrono>
#include <future>

#include <gtest/gtest.h>

#include "../async/internal/queuepool.h"

using namespace kors::async;

struct Store {
    std::vector<Queue*> queues;
    std::vector<std::thread::id> threadIds;
    std::mutex mutex;
};

TEST(QueuePool_Tests, Force)
{
    QueuePool* qp = QueuePool::instance();
    Store store;
    std::vector<std::thread> threads;
    std::atomic<bool> running = true;
    for (size_t i = 0; i < 50; ++i) {
        auto t = std::thread([qp, &store, &running]() {
            {
                std::scoped_lock<std::mutex> lock(store.mutex);

                const std::thread::id currentThreadId = std::this_thread::get_id();
                if (store.threadIds.empty()) {
                    store.threadIds.push_back(currentThreadId);
                    return;
                }
                const std::thread::id previousThreadId = store.threadIds.back();
                store.threadIds.push_back(currentThreadId);
                Queue* q = new Queue();
                store.queues.push_back(q);

                qp->regPort(previousThreadId, q->port1());
                qp->regPort(currentThreadId, q->port2());
            }

            while (running.load()) {
                qp->processMessages(std::this_thread::get_id());
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });

        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    running.store(false);

    for (auto& t : threads) {
        t.join();
    }
}

TEST(QueuePool_Tests, RegistrationWaitsForActiveProcessing)
{
    QueuePool* qp = QueuePool::instance();
    Queue activeQueue;
    Queue queueToRegister;
    const std::thread::id targetThread = std::this_thread::get_id();
    std::promise<void> handlerEnteredPromise;
    std::shared_future<void> handlerEntered = handlerEnteredPromise.get_future().share();
    std::promise<void> releaseHandlerPromise;
    std::shared_future<void> releaseHandler = releaseHandlerPromise.get_future().share();
    std::promise<void> registrationFinishedPromise;
    std::future<void> registrationFinished = registrationFinishedPromise.get_future();

    activeQueue.port1()->onMessage([&handlerEnteredPromise, releaseHandler](const CallMsg&) {
        handlerEnteredPromise.set_value();
        releaseHandler.wait();
    });
    qp->regPort(targetThread, activeQueue.port1());
    activeQueue.port2()->send(CallMsg {});

    std::thread processingThread([qp, targetThread]() {
        qp->processMessages(targetThread);
    });
    ASSERT_EQ(handlerEntered.wait_for(std::chrono::seconds(1)), std::future_status::ready);

    std::thread registrationThread([qp, targetThread, &queueToRegister, &registrationFinishedPromise]() {
        qp->regPort(targetThread, queueToRegister.port1());
        registrationFinishedPromise.set_value();
    });

    EXPECT_EQ(registrationFinished.wait_for(std::chrono::milliseconds(20)), std::future_status::timeout);

    releaseHandlerPromise.set_value();
    processingThread.join();
    EXPECT_EQ(registrationFinished.wait_for(std::chrono::seconds(1)), std::future_status::ready);
    registrationThread.join();

    qp->unregPort(targetThread, queueToRegister.port1());
    qp->unregPort(targetThread, activeQueue.port1());
}

TEST(QueuePool_Tests, HandlerCanRegisterPortForItsOwnThread)
{
    QueuePool* qp = QueuePool::instance();
    Queue activeQueue;
    Queue queueToRegister;
    const std::thread::id targetThread = std::this_thread::get_id();
    bool handlerCompleted = false;

    activeQueue.port1()->onMessage([qp, targetThread, &queueToRegister, &handlerCompleted](const CallMsg&) {
        qp->regPort(targetThread, queueToRegister.port1());
        handlerCompleted = true;
    });
    qp->regPort(targetThread, activeQueue.port1());
    activeQueue.port2()->send(CallMsg {});

    qp->processMessages(targetThread);

    EXPECT_TRUE(handlerCompleted);
    qp->unregPort(targetThread, queueToRegister.port1());
    qp->unregPort(targetThread, activeQueue.port1());
}

TEST(QueuePool_Tests, HandlerCanUnregisterPortForItsOwnThread)
{
    QueuePool* qp = QueuePool::instance();
    Queue activeQueue;
    Queue queueToUnregister;
    const std::thread::id targetThread = std::this_thread::get_id();
    bool handlerCompleted = false;

    activeQueue.port1()->onMessage([qp, targetThread, &queueToUnregister, &handlerCompleted](const CallMsg&) {
        qp->unregPort(targetThread, queueToUnregister.port1());
        handlerCompleted = true;
    });
    qp->regPort(targetThread, activeQueue.port1());
    qp->regPort(targetThread, queueToUnregister.port1());
    activeQueue.port2()->send(CallMsg {});

    qp->processMessages(targetThread);

    EXPECT_TRUE(handlerCompleted);
    qp->unregPort(targetThread, activeQueue.port1());
}
