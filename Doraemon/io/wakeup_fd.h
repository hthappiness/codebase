#pragma once

#include <memory>

class CWakeupFd
{
public:
    int read_fd;
    int write_fd;
    ;
};

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