
#include "eventfd.h"

/* 熟悉底层接口的用法，比如eventFd机制，基于此封装新的策略，描述对象的关系 */

int CEventFdImpl::init() 
{
  m_eventFdSet->read_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  m_eventFdSet->write_fd = -1;
  if (m_eventFdSet->read_fd < 0) 
  {
    return GRPC_OS_ERROR(errno, "eventfd");
  }
  return GRPC_ERROR_NONE;
}

int CEventFdImpl::wakeup_consume(grpc_wakeup_fd* fd_info) 
{
  eventfd_t value;
  int err;
  do 
  {
    err = eventfd_read(m_eventFdSet->read_fd, &value);
  } while (err < 0 && errno == EINTR);
  if (err < 0 && errno != EAGAIN) 
  {
    return GRPC_OS_ERROR(errno, "eventfd_read");
  }
  return GRPC_ERROR_NONE;
}

int CEventFdImpl::wakeup(grpc_wakeup_fd* fd_info) 
{
  GPR_TIMER_SCOPE("eventfd_wakeup", 0);
  int err;
  do 
  {
    err = eventfd_write(m_eventFdSet->read_fd, 1);
  } while (err < 0 && errno == EINTR);
  if (err < 0) 
  {
    return GRPC_OS_ERROR(errno, "eventfd_write");
  }
  return GRPC_ERROR_NONE;
}

void CEventFdImpl::eventfd_destroy(grpc_wakeup_fd* fd_info) 
{
  if (m_eventFdSet->read_fd != 0)
  { 
      close(m_eventFdSet->read_fd);
  }
}

int CEventFdImpl::check_availability(void) 
{
  const int efd = eventfd(0, 0);
  const int is_available = efd >= 0;
  if (is_available)
  { 
    close(efd);
  }
  return is_available;
}