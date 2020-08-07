
#pragma once

#include <list>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace Doreamon
{
    namespace base{
        template<typename T>
        class CBoundedBlockingQueue {
            public:
                CBoundedBlockingQueue(int iMaxSize = 100) : m_iMaxSize(iMaxSize) , m_running(true)
                {

                }

                void put(const T& task)
                {
                    add(task);
                }

                void put(const T&& task)
                {
                    add(std::forward<T>(task));
                }

                void take(std::list<T>& list)
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_notEmptyCv.wait(lock, [this]{
                        return !m_running || !m_queue.empty();
                    });

                    if(!m_running)  
                        return;

                    list = std::move(m_queue);

                    m_notFullCv.notify_one();
                }

                 void take( T& t)
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_notEmptyCv.wait(lock, [this]{
                        return !m_running || !m_queue.empty();
                    });

                    if(！m_running)  
                        return;

                    t = m_queue.pop_front();

                    m_notFullCv.notify_one();
                }

                void stop()
                {
                    m_running = false;

                    m_notFullCv.notify_all();
                    m_notEmpty.notify_all();
                }
            private:
                template<typename Type>
                void add(Type&& task)
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_notFullCv.wait(lock, [this](
                        return !m_running || m_queue.size() < m_iMaxSize; 
                    ));

                    if( !m_running ) 
                        return;

                    m_queue.push_back(std::forward<Type>(task));

                    m_notEmptyCv.notify_one();
                }
            
            private：
                std::list<T> m_queue;
                std::mutex m_mutex;
                std::condition_variable m_notEmptyCv;
                std::condition_variable m_notFullCv;

                int m_iMaxSize;
                std::atomic_bool m_running;
        };
    }
}