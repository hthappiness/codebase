
#pragma once

#include "wakeup_fd.h"

class CEventFdImpl : public CWakeupInterface
{
public:
    virtual int init(CWakeupFd *fd_info);
    virtual int wakeup(CWakeupFd *fd_info);
    virtual void destroy(CWakeupFd *fd_info);
    /* Must be called before calling any other functions */
    virtual int check_availability(void);

private:
    CWakeupFd m_eventFdSet;
} ;