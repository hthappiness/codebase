#include "buffer.h"


int CSender::pcivSendData(char* pBuf, int ilen)
{
    /* to do */

    if( !m_PcivBuf.IsValid())
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        auto now = std::chrono::system_clock::now();
        /* 100ms释放一次锁，直到条件满足阻塞才会解除 */
        cv.wait_until(lk, now + 100ms, [](){return m_PcivBuf.IsValid();}
        /*
            //http://www.cplusplus.com/reference/condition_variable/condition_variable/wait_until/
            //just like this.
            while (!pred())
                if ( wait_until(lck,abs_time) == cv_status::timeout)
                    return pred();
        */
    }

    /* to do */
}