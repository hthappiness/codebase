

#include <atmoic>
#include <future>

#include "bounded_blocking_queue.h"

namespace Doraemon{
    namespace base{
        class CThreadPool{
            using CTask = std::function<void()>;

            CThreadPool();

            virtual ~CThreadPool();

            void start(int iThreadCnt);

            void stop();

            template<typename F, typename... Args>
            auto postTask(F&& func, Args... args)->std::future<typename std::result_of<F(Args...)>::type>;

        private:
            std::list<std::thread> m_workers;
            CBoundedBlockingQueue<CTask> m_tasks;
            std::atomic_bool m_running;

            std::once_flag m_stop_flag;
        };

        template<typename F, typename... Args>
        auto postTask(F&& func, Args... args)->std::future<typename std::result_of<F(Args...)>::type>
        {
            using return_type = typename std::result_of<F(Args...)>::type;

            auto task = std::make_shared<std::packaged_task<return_type()>>)(
                std::bind(std::forward<F>(func), std::forward<Args>(args)...)
            );

            m_tasks.put([task] {(*task)()};);

            return ret;
        }
    }
}