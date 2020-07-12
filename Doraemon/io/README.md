
self-consistent software system.自洽的软件系统

The document is about the description of the input/output module.
This will be seperated into several parts below.
1. module interface

2. encapsule the epoll select and poll.
   abstract the system poller operation

3. design pattern: reactor, actor, proactor

4. abstract io object and io operation, socket object , linux file descriptor----------->driver fd
   


about io
1. io core

2. io object
  socket
  fd: eventfd, driver, pipe, serial

3. multi io
  poll epoll select

4. platform
   windows
   linux: posix 