// 5.10
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <assert.h>

#include <iostream>

//在理论（理念和特征）上，讨论原子性（事务特征），有序性（），可见性（），等等。
//而在工程实现上，一个接口就完成了所有功能，也是可以的；也就有了不同层面的东西，硬件层面---内存屏障、汇编层面、os层面、库层面
// 关注其接口

/*
Value	Explanation
    memory_order_relaxed	Relaxed operation: there are no synchronization or ordering constraints imposed on other reads or writes, only this operation's atomicity is guaranteed (see Relaxed ordering below)
    memory_order_consume	A load operation with this memory order performs a consume operation on the affected memory location: no reads or writes in the current thread dependent on the value currently loaded can be reordered before this load. Writes to data-dependent variables in other threads that release the same atomic variable are visible in the current thread. On most platforms, this affects compiler optimizations only (see Release-Consume ordering below)
    memory_order_acquire	A load operation with this memory order performs the acquire operation on the affected memory location: no reads or writes in the current thread can be reordered before this load. All writes in other threads that release the same atomic variable are visible in the current thread (see Release-Acquire ordering below)
    memory_order_release	A store operation with this memory order performs the release operation: no reads or writes in the current thread can be reordered after this store. All writes in the current thread are visible in other threads that acquire the same atomic variable (see Release-Acquire ordering below) and writes that carry a dependency into the atomic variable become visible in other threads that consume the same atomic (see Release-Consume ordering below).
    memory_order_acq_rel	A read-modify-write operation with this memory order is both an acquire operation and a release operation. No memory reads or writes in the current thread can be reordered before or after this store. All writes in other threads that release the same atomic variable are visible before the modification and the modification is visible in other threads that acquire the same atomic variable.
    memory_order_seq_cst	A load operation with this memory order performs an acquire operation, a store performs a release operation, and read-modify-write performs both an acquire operation and a release operation, plus a single total order exists in which all threads observe all modifications in the same order (see Sequentially-consistent ordering below)#endif
影响线程之间的可见性和顺序性reorder的逻辑
*/

static int i=0;

//static std::atomic<int> i{0};  //atomic能保证可见性？

void operation()
{
    i++;
}

int main()
{
    std::vector<std::thread> vecThread;
    for( int j = 0 ; j < 100 ; j++)
    {
        vecThread.emplace_back(operation);
    }

    for(auto& thread : vecThread)
    {
        thread.join();
    }

    //std::cout << "result is : " << i << std::endl;
    if( i != 100)
    {
     std::cout << "result is : " << i << std::endl;
    }
    assert( i==100);
}
