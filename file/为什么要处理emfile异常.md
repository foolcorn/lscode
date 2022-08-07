### 为什么要处理emfile异常

虽然accept了，但是因为emfile失败了，失败返回的clientfd无法关闭close

clientfd无法关闭，epoll就会一直空转显示有可读事件。



线程真实pid tid

syscall(SYS_getttid)

