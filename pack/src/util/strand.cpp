//#pragma once

//#include "Monitor.h"
#include <assert.h>
#include <queue>
#include <functional>
 
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "callStack.hpp"

class WorkQueue {
public:
    // Add a new work item
    template <typename F>
    void push(F w) {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_q.push(std::move(w));
        m_cond.notify_all();
    }
 
    // Continuously waits for and executes any work items, until "stop" is
    // called
    void run() {
        //Callstack<WorkQueue>::Context ctx(this);
        while (true) {
            std::function<void()> w;
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                m_cond.wait(lock, [this] { return !m_q.empty(); });
                w = std::move(m_q.front());
                m_q.pop();
            }
 
            if (w) {
                w();
            } else {
                // An empty work item means we are shutting down, so enqueue
                // another empty work item. This will in turn shut down another
                // thread that is executing "run"
                push(nullptr);
                return;
            }
        };
    }
 
    // Causes any calls to "run" to exit.
    void stop() {
        push(nullptr);
    }
 
    // Tells if "run" is executing in the current thread
    //bool canDispatch() {
    //   return Callstack<WorkQueue>::contains(this) != nullptr;
    //}
 
private:
    std::condition_variable m_cond;
    std::mutex m_mtx;
    std::queue<std::function<void()>> m_q;
};

template <class T>
class Monitor {
private:
    mutable T m_t;
    mutable std::mutex m_mtx;

public:
    using Type = T;
    Monitor() {}
    Monitor(T t_) : m_t(std::move(t_)) {}
    template <typename F>
    auto operator()(F f) const -> decltype(f(m_t)) {
        std::lock_guard<std::mutex> hold{m_mtx};
        return f(m_t);
    }
}; 
 
//
// A strand serializes handler execution.
// It guarantees the following:
// - No handlers executes concurrently
// - Handlers are only executed from the specified Processor
// - Handler execution order is not guaranteed
//
// Specified Processor must implement the following interface:
//
//  template <typename F> void Processor::push(F w);
//      Add a new work item to the processor. F is a callable convertible
// to std::function<void()>
//
//  bool Processor::canDispatch();
//      Should return true if we are in the Processor's dispatching function in
// the current thread.
//类和模板的组合，对Process的约束只是几个函数，不同于非模板类（普通class）的聚合，需要统一的基类才能有类似的效果；模板更抽象了
//模板，不显式的耦合，对其实例化的约束较少，毕竟生成了一个新的类
//类参数的耦合，函数参数的耦合
template <typename Processor>
class Strand {
public:
    Strand(Processor& proc) : m_proc(proc) {}
 
    Strand(const Strand&) = delete;
    Strand& operator=(const Strand&) = delete;
 
    // Executes the handler immediately if all the strand guarantees are met,
    // or posts the handler for later execution if the guarantees are not met
    // from inside this call
    template <typename F>
    void dispatch(F handler) {
        // If we are not currently in the processor dispatching function (in
        // this thread), then we cannot possibly execute the handler here, so
        // enqueue it and bail out
        if (!m_proc.canDispatch()) {
            post(std::move(handler));
            return;
        }
 
        // NOTE: At this point we know we are in a worker thread (because of the
        // check above)
 
        // If we are running the strand in this thread, then we can execute the
        // handler immediately without any other checks, since by design no
        // other threads can be running the strand
        if (runningInThisThread()) {
            handler();
            return;
        }
 
        // At this point we know we are in a worker thread, but not running the
        // strand in this thread.
        // The strand can still be running in another worker thread, so we need
        // to atomically enqueue the handler for the other thread to execute OR
        // mark the strand as running in this thread
        auto trigger = m_data([&](Data& data) {
            if (data.running) {
                data.q.push(std::move(handler));
                return false;
            } else {
                data.running = true;
                return true;
            }
        });
 
        if (trigger) {
            // Add a marker to the callstack, so the handler knows the strand is
            // running in the current thread
            typename Callstack<Strand>::Context ctx(this);
            handler();
 
            // Run any remaining handlers.
            // At this point we own the strand (It's marked as running in
            // this thread), and we don't release it until the queue is empty.
            // This means any other threads adding handlers to the strand will
            // enqueue them, and they will be executed here.
            run();
        }
    }
 
    // Post an handler for execution and returns immediately.
    // The handler is never executed as part of this call.
    template <typename F>
    void post(F handler) {
        // We atomically enqueue the handler AND check if we need to start the
        // running process.
        bool trigger = m_data([&](Data& data) {
            data.q.push(std::move(handler));
            if (data.running) {
                return false;
            } else {
                data.running = true;
                return true;
            }
        });
 
        // The strand was not running, so trigger a run
        if (trigger) {
            m_proc.push([this] { run(); }); //给workQueue的是这个run 函数，而不是用户post的函数
        }
    }
 
    // Checks if we are currently running the strand in this thread
	// 多线程处理技巧
    bool runningInThisThread() {
        return Callstack<Strand>::contains(this) != nullptr;
    }
 
private:
    // Processes any enqueued handlers.
    // This assumes the strand is marked as running.
    // When there are no more handlers, it marks the strand as not running.
    // 工作线程和调用线程会调用这个接口
    void run() {
        typename Callstack<Strand>::Context ctx(this);
        while (true) {
            std::function<void()> handler;
            m_data([&](Data& data) {
                assert(data.running);
                if (data.q.size()) {
                    handler = std::move(data.q.front());
                    data.q.pop();
                } else {
                    data.running = false;
                }
            });
 
            if (handler)
                handler();
            else
                return;
        }
    }
 
    struct Data {
        bool running = false;
        std::queue<std::function<void()>> q;
    };
    Monitor<Data> m_data;
    Processor& m_proc;
};


//#include "Strand.h"
//#include "WorkQueue.h"
#include <random>
#include <stdlib.h>
#include <string>
#include <atomic>
 
// http://stackoverflow.com/questions/7560114/random-number-c-in-some-range
int randInRange(int min, int max) {
    std::random_device rd;   // obtain a random number from hardware
    std::mt19937 eng(rd());  // seed the generator, operator func,运算符()重载
    std::uniform_int_distribution<> distr(min, max);  // define the range
    return distr(eng);
}
 
struct Obj {
    explicit Obj(int n, WorkQueue& wp) : strand(wp) {
        name = "Obj " + std::to_string(n);
    }
 
    void doSomething(int val) {
        printf("\r\n %s : doing %d\r\n", name.c_str(), val);
    }
    std::string name;
    Strand<WorkQueue> strand;
};
 
//多个strand，提交任务
//多个workerThread， 处理任务
//该运行模型保证的是，同一个strand里的任务不会并发执行，根据调用栈判断
void strandSample() {
    WorkQueue workQueue;
    // Start a couple of worker threads
    std::vector<std::thread> workerThreads;
    for (int i = 0; i < 4; i++) {
        workerThreads.push_back(std::thread([&workQueue] { workQueue.run(); }));
    }
 
    // Create a couple of objects that need strands
    std::vector<std::shared_ptr<Obj>> objs;
    for (int i = 0; i < 8; i++) {
        objs.push_back(std::make_shared<Obj>(i, workQueue));
    }
 
    // Counter used by all strands, so we can check if all work was done
    std::atomic<int> doneCount(0);
 
    // Add work to random objects
    const int todo = 20;
    for (int i = 0; i < todo; i++) {
        auto&& obj = objs[randInRange(0, objs.size() - 1)];
        obj->strand.post([&obj, i, &doneCount] {
            obj->doSomething(i);
            ++doneCount;
        });
    }
 
    workQueue.stop();
    for (auto&& t : workerThreads) {
        t.join();
    }
 
    assert(doneCount == todo);
}

int main()
{
	strandSample();
	return 0;
}


//g++ -std=c++11 strand.cpp -o strand -pthread
//./strand  |grep "Obj 6"