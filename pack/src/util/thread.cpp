
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

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
 
int main() {
    WorkQueue work;
 
    // Start a couple of consumer threads
    std::vector<std::thread> ths;
    for (int i = 0; i < 4; i++) {
        ths.push_back(std::thread([&work] { work.run(); }));
    }
 
    std::vector<int> res;
    res.resize(10);
 
    // Enqueue work.
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
        // These work items simply increment the element at index i.
        auto func = [i, &res] {res[i] += 2;};
	work.push(func);
        //work.push([i, &res] { res[i] += 2; });
    }
 
    // Stop consumers, and wait for the threads to finish
    work.stop();
    for (auto&& t : ths) t.join();
 
    // Test if all work items were executed
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
        if (res[i] != 1)
            printf("ERROR: Index %d set to %d\n", i, res[i]);
    }

    std::cout<<"finish the main function!"<<std::endl;	
    return 0;
}
