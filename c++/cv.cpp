#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable
#include <chrono>

std::mutex mtx; // 全局互斥锁.
std::condition_variable cv; // 全局条件变量.
bool ready = false; // 全局标志位.

void do_print_id(int id)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::unique_lock <std::mutex> lck(mtx);
    //while (!ready){ // 如果标志位不为 true, 则等待...
    //cv.wait(lck); // 当前线程被阻塞, 当全局标志位变为 true 之后,
    //std::cout<<"waiting"<<std::endl;
    //}
    cv.wait(lck);
    // 线程被唤醒, 继续往下执行打印线程编号id.
    std::cout << "thread " << id << '\n';
}
    
void go()
{
    //std::unique_lock <std::mutex> lck(mtx);
    ready = true; // 设置全局标志位为 true.
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<<"notify one!"<<std::endl;
    cv.notify_all(); // 唤醒所有线程.
}
                    
int main()                    
{                        
    std::thread threads[10];
    // spawn 10 threads:
    for (int i = 0; i < 10; ++i)
         threads[i] = std::thread(do_print_id, i);
    
    std::cout << "\033[5;36;43m 10 threads ready to race...\33[0m \n";
    for (int j = 0;j < 10;j++)
    {
        //std::cout<<"go:"<<j<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));    
	go(); // go!
    }
    for (auto & th:threads)
          th.join();
    
    return 0;
}
