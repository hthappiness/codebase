
#include <chrono>
#include <iostream>

namespace Doraemon
{

    class ProfileScope
    {
    public:
        ProfileScope(const char *desc, bool important, const char *file, int line)
            : desc_(desc), file_(file), line_(line)
        {
            m_beginTime = std::chrono::steady_clock::now();
        }
        ~ProfileScope() { 
            auto costTime = std::chrono::duration_cast<std::chrono::milliseconds>
		            (std::chrono::steady_clock::now() - m_beginTime);
	
	        std::cout << "[" <<file_ << ": "<< line_ << "]" << costTime.count() << "] ms" << std::endl;
         }

    private:
        const char *const desc_;
        const char *const file_;
        int line_;
        std::chrono::steady_clock::time_point m_beginTime;
    };

    //C++中，宏定义不受命名空间的约束
    #define DORAEMON_TIMER_SCOPE(tmp, level) \
        ProfileScope tmp(desc, level, __FILE__, __LINE__);
}