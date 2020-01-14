
#include <iostream>
#include <chrono>

/*
The ulitity about time ,the function ,the class, the object, the operation, the variables
These resources are well organized by the c++ library.

1、基本功能的正交
2、好用、直观的接口
3、

OOP + template 的组织方式

std::chrono::stready_clock ;
std::chrono::system_clock ;
std::chrono::high_resolution_lock ;

Durations
They measure time spans, like: one minute, two hours, or ten milliseconds.

Time points
A reference to a specific point in time, like one’s birthday, today’s dawn, or when the next train passes.

Clocks
A framework that relates a time point to real physical time.

https://blog.csdn.net/mo4776/article/details/80116112

*/
#include <iostream>
#include <chrono>
#include <ratio>
#include <thread>
void f()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
 
int test()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    f();
    auto t2 = std::chrono::high_resolution_clock::now();
 
    // 整数时长：要求 duration_cast
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
 
    // 小数时长：不要求 duration_cast
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
 
    std::cout << "f() took " << fp_ms.count() << " ms, "
              << "or " << int_ms.count() << " whole milliseconds\n";
}
//可能的输出：
//f() took 1000.23 ms, or 1000 whole milliseconds

class CTimeStat
{
public:
    const char* m_tag;
    std::chrono::steady_clock::time_point m_startTp = std::chrono::steady_clock::now();
public:
    CTimeStat(const char* tag):
        m_tag(tag){

    }

    ~CTimeStat()
    {
        auto elapsedClk = std::chrono::stready_clock::now() - m_startTp;
        auto elapsedTm  = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedClk);
        std::cout<<m_tag<<"elapsed Time:"<<elapsedTm<<std::endl;
    }
};

template <typename T>
class CPrintCfg
{
public:
    using Handle = std::function<void(T&&)>;
private:
    int iInterval;
    int iIntervalCnt;

    CPrintCfg(handle print)
    {
        m_print = print;
    }

    ~CPrintCfg(){}

    void operator() ()
    {
        iInterValCnt++;
        if( iIntervalCnt % iInterval)
        {
            m_print();
        }
    }

    Handle m_print;
};

template <typename T>
class CBufferConsumer : public boost::noncopyable
{
    using Handler = std::function<void(T&&)>;
public:
    CBufferConsumer(unsigned long ulCapacity)
        :m_queue(ulCapacity)
    {

    }

    void push(const T& value)
    {
        m_queue.push_back(value);
    }

    void push(T&& value)
    {
        m_queue.push_back(std::move(value));
    }

    void run(const Handler& handler, const std::function<void()>& pre = nullptr)
    {
        m_process = handler;
        m_pre = pre;
        m_thread = std::thread(&CBufferConsumer::work, this);
    }

    void join()
    {
        m_queue.close();
        m_thread.join();
    }
private:

    void work()
    {
            if( m_pre )
            {
                m_pre();
            }

            while(true)
            {
                auto status = m_queue.wait_pull_front(m_buffer);
                if ( boost::concurrency::queue_op_status::success == status )
                {
                        m_process(std::move(m_buffer));
                        m_buffer = {};
                }
                else if ( boost::concurrency::queue_op_status::closed == status )
                {
                    break;
                }
            }

    }

private:
    boost::concurrency::sync_bounded_queue<T> m_queue;

    std::function<void()> m_pre;
    Handler m_process;
    std::thread m_thread;
    T m_buffer;
};

class CFinalGuard
{
public:
    CFinalGuard(const std::function<void()>& final)
        :m_final(final)
    {}
    ~CFinalGuard()
    {
        if ( false == m_bCanceled)
        {
            try{
                m_final();
            }
            catch(...)
            {
                
            }
        }
    }

    void cancel() noexcept;
private:
    const std::function<void()> m_final;
    bool m_bCanceled = false;
}

#ifndef ASIO_TIMER_H  
#define ASIO_TIMER_H  
typedef std::function<void()> ProcessFun;
//以steady_timer替代原来的deadline_timer
typedef boost::shared_ptr <boost::asio::steady_timer> pSteadyTimer;
struct STimerUnit
{
    int id;
    int seconds;
    pSteadyTimer t;
    ProcessFun fun;
};

typedef boost::shared_ptr<STimerUnit> TimerUnitPtr;
class CTimer
{
public:
    CTimer() :m_ioWork(m_ioService), m_lID(0)
    {
    }

public:

    //添加一个定时业务,f为业务处理函数,arg为自定义参数，seconds为超时秒数  
    //返回生成的ID  
    int AddTimerUnit(ProcessFun f,int seconds);
    //每intervalSeconds秒数执行一次 f函数  
    int AddTimerIntervalUnit(ProcessFun f, int intervalSeconds);
    //删除指定定时器  
    void RemoveTimerUnit(int id);

    bool TimerisValid(int id);
    void Run();

private:
    void TimerProcess(int id, bool isIntervalTimer, const boost::system::error_code& e);

private:
    std::map<int, TimerUnitPtr> m_mapTimerUnits;
private:
    boost::asio::io_service m_ioService;
    boost::asio::io_service::work m_ioWork;

private:
    std::mutex m_mutexTimerUnit;

private:
    //分配timer id  
    std::vector<int> m_vecTimerUnitIDs;
    unsigned long long m_lID;
};
#endif 

#include "log.h"  
#include "asiotimer.h"  
  
int CTimer::AddTimerUnit(ProcessFun f,int seconds)
{
    TimerUnitPtr s(new STimerUnit);
    s->seconds = seconds;
    s->t.reset(new boost::asio::steady_timer(m_ioService, std::chrono::seconds(seconds)));
    s->fun = f;

    {
        std::lock_guard<std::mutex> lock(m_mutexTimerUnit);
        m_mapTimerUnits.insert(std::make_pair(++m_lID, s));
        s->t->async_wait(boost::bind(&CTimer::TimerProcess, this, m_lID,false, boost::asio::placeholders::error));
        return m_lID;
    }
}

int CTimer::AddTimerIntervalUnit(ProcessFun f, int intervalSeconds)
{
    TimerUnitPtr s(new STimerUnit);
    s->seconds = intervalSeconds;
    s->t.reset(new boost::asio::steady_timer(m_ioService, std::chrono::seconds(intervalSeconds)));
    s->fun = f;

    {
        std::lock_guard<std::mutex> lock(m_mutexTimerUnit);
        m_mapTimerUnits.insert(std::make_pair(++m_lID, s));
        s->t->async_wait(boost::bind(&CTimer::TimerProcess, this, m_lID, false, boost::asio::placeholders::error));
        return m_lID;
    }
}

void CTimer::RemoveTimerUnit(int id)
{
    std::lock_guard<std::mutex> lock(m_mutexTimerUnit);
    std::map<int, TimerUnitPtr>::iterator It = m_mapTimerUnits.find(id);
    if (It != m_mapTimerUnits.end())
    {
        It->second->t->cancel();
        m_mapTimerUnits.erase(It);
        return;
    }
}

bool CTimer::TimerisValid(int id)
{
    std::lock_guard<std::mutex> lock(m_mutexTimerUnit);
    std::map<int, TimerUnitPtr>::iterator It = m_mapTimerUnits.find(id);
    if (It != m_mapTimerUnits.end())
    {
        return true;
    }

    return false;
}

void CTimer::Run()
{
    m_ioService.run();
}

void CTimer::TimerProcess(int id, bool isIntervalTimer, const boost::system::error_code& e)
{
    if (e == boost::asio::error::operation_aborted)
    {
        return;
    }

    TimerUnitPtr pTimerUnit;

    {
        std::lock_guard<std::mutex> lock(m_mutexTimerUnit);
        std::map<int, TimerUnitPtr>::iterator It = m_mapTimerUnits.find(id);
        if (It != m_mapTimerUnits.end())
        {
            pTimerUnit = It->second;
            if (!isIntervalTimer)
            {
                m_mapTimerUnits.erase(It);
            }
        }

        //LOG_INFO << "=========>mapTimerUnits size " << m_mapTimerUnits.size() << std::endl;
    }

    if (pTimerUnit)
    {
        pTimerUnit->fun();
        if (isIntervalTimer)
        {
            pTimerUnit->t->expires_at(pTimerUnit->t->expires_at() + std::chrono::seconds(pTimerUnit->seconds));
            pTimerUnit->t->async_wait(boost::bind(&CTimer::TimerProcess, this, id,true, boost::asio::placeholders::error));
        }
    }
    else
    {
        //LOG_INFO << "TimerUnit pointer is NULL" << std::endl;
    }
}
