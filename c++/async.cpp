

#include <iostream>
#include <thread>
#include <future>
#include <functional>
#include <chrono>

#include <sstream>
#include <string>

#include <initializer_list>

/* packaged_task的简易实现；只是标准库中模板设置的更好，
 *
 * 根据调用对象的类型，推理返回值类型，这个就更为抽象，这个实现只是packaged_task的阉割版，子集 
 */
template<typename T>
class CAsyncResult
{
    using funcHandler = std::function<T()>;
public:
    CAsyncResult(std::initializer_list<int>& list)
    {
        std::stringstream ss;
        std::initializer_list<int>::iterator iter;
        for(iter = list.begin(); iter != list.end() ; iter++ )
        {
             std::cout << " " <<  *iter;
             ss << ' ' << *iter ;
        }

        std::cout<< ss.str();
        
        #if 0
        for(auto& item : list)
        {
            std::cout << item;
        }
        #endif

        std::cout << std::endl;
    }
    
    //委托构造函数，两个构造函数如果同时可以适配一个实参，则有歧义
    CAsyncResult(const std::initializer_list<int>& list)
    {
        CAsyncResult(const_cast<std::initializer_list<int>&>(list));
    }

    #if 0
    template<typename F, typename... Args>
    std::future<T> postTask(F, Args)
    {
        //async task queue
        return m_promise.get_future();
    }
    #endif

    std::future<T> get_future()
    {
         return m_promise.get_future();
    }

    void start(const funcHandler& f)
    {
        std::thread{[&](){
            T ret = f();
           
            std::this_thread::sleep_for(std::chrono::seconds(1));

            setResult(ret);
        }
        }.join();
    }

private:
    void setResult(T& result)
    {
        m_promise.set_value(result);
    }
private:
    std::promise<T> m_promise;  
    //future, It is cannot be copyed. shared_future
};

int main()
{
    std::cout << "***********************************************" << std::endl;
    std::cout << "--------------It is promised future!------------" << std::endl;
    
    CAsyncResult<int> test({3, 5, 2, 9});
    int i = 5*5;
    std::future<int> future = test.get_future();
    
    auto f = [&i]() -> int
    {
        return i;
    };

    test.start(f);

    std::future_status status = future.wait_for(std::chrono::seconds(10));
    if( status == std::future_status::timeout )
    {
        std::cout << "get var timeout, drop it.";
        return 0;
    }

    int ret = future.get();

    std::cout << "result = " << ret << std::endl;

    std::cout << "***********************************************" << std::endl;
    std::cout << "--------------It is packaged task!-------------" << std::endl;
    
    i*=3;
    std::packaged_task<int ()> task(f);

    auto future2 = task.get_future();

    //std::thread(std::move(task)).join();
    std::thread trd(std::move(task));

    int ret2 = future2.get();

    std::cout << "result2 = " << ret2 << std::endl;

    trd.join();

    {
        std::cout << "***********************************************" << std::endl;
        std::cout << "--------------It is async !--------------------" << std::endl;

        auto get_value = []() { return 10; };

        // start a new thread
        //std::future<int> fut = std::async (std::launch::async, get_value);

        std::future<int> fut = std::async (get_value);
        std::shared_future<int> shfut = fut.share();

        try
        {
            std::cout << "after share value: " << fut.get() << '\n';
        }
        catch(const std::future_error& e) //std::future_error
        {
            //属于future 范畴的异常
            if (e.code().category() == std::future_category())
            {
                std::cerr << e.what() << '\n';
            }
        }
    
        // shared futures can be accessed multiple times:
        std::cout << "value: " << shfut.get() << '\n';
        std::cout << "its double: " << shfut.get()*2 << '\n';

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}