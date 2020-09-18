#pragma once

#include <memory>

/*
 *结合epoll使用，wakeup的时候就向readfd写入，以通知write fd，notify之后，就可以做相应的操作
 *定义用法，也就是接口，再谈具体实现
 */

class CWakeupFd
{
public:
    int read_fd;
    int write_fd;
    ;
};

/* 可以由eventFd实现，或者其他机制 */
class CWakeupInterface
{
public:
    virtual int init(CWakeupFd *fd_info) = 0;
    virtual int wakeup(CWakeupFd *fd_info) = 0;
    virtual void destroy(CWakeupFd *fd_info) = 0;
    /* Must be called before calling any other functions */
    virtual int check_availability(void) = 0;
};

class CWakeupWrapper
{
private:
    std::unique_ptr<CWakeupInterface> m_wakeupImpl;
};