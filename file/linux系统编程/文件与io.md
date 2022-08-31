打开或者创建一个文件的时候，内核会维护打开文件描述



进程默认三个文件描述符：int类型

STDIN_FILENO 0

STDOUT_FILENO 1

STDERR_FILENO 2



stdin，stdout，stderr是文件指针





### posix接口

#### fdopen

![image-20220810233458395](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810233458395.png)

fdopen可以将文件描述符转成文件指针

#### open

![image-20220810233901290](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810233901290.png)



![image-20220810233604915](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810233604915.png)

flags：

![image-20220810234017474](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810234017474.png)

mode：

![image-20220810234134580](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810234134580.png)



#### close

![image-20220810234717025](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810234717025.png)



#### creat

![image-20220810234739928](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810234739928.png)

创建文件，现在基本不用了，已经被open替代了、



#### read

![image-20220810234831620](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810234831620.png)

#### write

![image-20220810234858387](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810234858387.png)

write返回非负数，只是表示成功写入内核缓冲区中，不代表刷入到磁盘

除非open的时候加入o_sync或者调用fsync接口



#### lseek

![image-20220810235746100](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810235746100.png)

![image-20220810235812399](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810235812399.png)



#### opendir

![image-20220810235923379](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220810235923379.png)

#### readdir

![image-20220811000654966](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220811000654966.png)

循环调用可以遍历目录，返回一个个结构体

![image-20220811000727178](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220811000727178.png)

- inode

- 偏移
- 文件名长度
- 文件名

#### mkdir

![image-20220811000140135](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220811000140135.png)



#### rmdir

![image-20220811000219963](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220811000219963.png)

#### chmod和fchmod

![image-20220811000237307](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220811000237307.png)

#### stat

![image-20220811001028685](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220811001028685.png)

![image-20220811001053009](C:\Users\郑毓铭\AppData\Roaming\Typora\typora-user-images\image-20220811001053009.png)

- stdev：磁盘设备号
- innode
- 

对于每个文件描述符，内核都会维护一个打开文件表，记录标志（读写，追加，同步，是否阻塞），偏移量，v节点指针

节点表则维护了v节点的信息和inode信息
