设计应用层的可靠udp传输



首先研究内核的协议栈

网卡数据进

sk_buffer

copy进协议栈

在copy到用户态



两次copy，开销比较大

零拷贝技术

网卡mmap到内存里。底层是dma（cpu不干预，相当于不消耗资源，不算copy）

应用层可以直接获取。



从网卡中取完整的数据

raw socket

netmap

dpdk



负载均衡

nginx：应用层

haproxy：传输层

lvs：网络层

f5：数据链路层。



优点：完全异步，减少内核拷贝

需要解决 

分片问题

底层数据包内存对齐的问题

应用层实现滑动窗口，进行发送速度的控制。

应用层需要实现epoll多路复用。

要自己实现底层协议

arp

icmp





