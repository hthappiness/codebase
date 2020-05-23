
#include "thread_pool.h"

namespace Doraemon{
    namespace base{
        CThreadPool::CThreadPool() : m_tasks(MAX_TASKS_COUNT)
        {

        }

        CThreadPool::~CThreadPool()
        {
            stop();
        }

        CThreadPool::start(int iNumThreads)
        {
            m_running = true;
            for(int i = 0 ;i < iNumThreads; i++)
            {
                m_workers.emplace_back(
                    [this] {
                        while (m_running) {
                            std::list<CTask> list;
                            m_tasks.task(list);
                            for(auto& task : list){
                                if( !m_running)
                                    return;

                                task();
                            }
                        }
                    }
                );
            }
        }

        void CThreadPool::stop()
        {
            std::call_once(m_stop_flag, [this]{
                m_tasks.stop();
                m_running = false;

                for(auto& worker : m_workers)
                {
                    worker.join();
                }

                m_workers.clear();
            });
        }
    }
}