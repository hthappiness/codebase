

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
            //boost::synced_bounded_queue<CTask> m_tasks;
            //boost::concurrent::queue_op_status::busy，从符号上来讲这个学习成本多大，虽然IDE能助力不少
            //wait_push_back, try_push_back
            std::atomic_bool m_running;

            std::once_flag m_stop_flag;
        };

        /*关于c++的抽象
         *0、c++的语法单元；命名空间、类型、变量、函数名，关键字
         *1、描述抽象语义的语言形式，语言级的抽象，标准库的抽象，提供何种抽象功能，如何建模对象
         *2、类型的操作
         *3、比如下面的F, 可调用对象作为模板参数，本身就没有对F本身做太多限制，所以根本不知道F的返回值，
         *   所以，需要一个功能，根据调用对象获取其返回值类型
         *4、函数抽象，闭包，结果抽象，；同时封装promise--->set_value、future--->get
             auto ret = postTask();
             ret.get()
             std::packaged_task, std::async
         *5、c++提供的接口，确实有些不友善，参数、使用都不是那么直接，而且组合形式多，再结合复杂的语法、机制，容易出错
         */
        template<typename F, typename... Args>
        auto postTask(F&& func, Args... args)->std::future<typename std::result_of<F(Args...)>::type>
        {
            using return_type = typename std::result_of<F(Args...)>::type;

            auto task = std::make_shared<std::packaged_task<return_type()>>)(
                std::bind(std::forward<F>(func), std::forward<Args>(args)...)
            );

            std::future<return_type> ret = task->get_future();

            m_tasks.put([task] {(*task)()};);

            return ret;
        }
    }
}