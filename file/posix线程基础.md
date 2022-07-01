## posix线程基础



### 链接线程库，

因为thread是静态库，需要手动链接

![image-20220627145018137](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627145018137.png)



### 创建线程

![image-20220627135005014](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627135005014.png)

arg是一个结构体，包含传进start_routine所有参数

![image-20220627135627221](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627135627221.png)



### 等待线程结束

![image-20220627150311760](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627150311760.png)

![image-20220627145305089](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627145305089.png)

retval不为空则会保存线程的退出状态

返回值成功0，否则返回errno





### 线程终止

![image-20220627145725976](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627145725976.png)

在线程函数中return和pthread_exit是一样的

僵尸线程：主线程没有join，但是子线程已经运行结束。可以使用pthread_detach脱离主线程



### 获取线程id

![image-20220627150403398](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627150403398.png)



### 一个线程取消另一个线程

![image-20220627150429895](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627150429895.png)



### 初始化线程属性

![image-20220627151337300](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627151337300.png)

有哪些属性可以设置：

- 是否分离（detach）：
  - ![image-20220627151435198](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627151435198.png)
  - ![image-20220627151611782](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627151611782.png)

- 线程的栈大小

  - ![image-20220627151653126](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627151653126.png)
  - stacksize设置为0，可以使用系统默认的栈大小

- 栈溢出保护区大小

  - ![image-20220627151844268](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627151844268.png)

- 线程竞争范围：

  - ![image-20220627152039199](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627152039199.png)

  - ![image-20220627152204412](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627152204412.png)

  - 竞争范围scope：描述了线程为处理资源而竞争的方式，系统竞争范围意味着线程与进程之外的线程竞争处理器资源，一个进程内的高优先级系统竞争范围线程能阻止其他进程内的系统竞争范围运行。竞争范围指的是仅仅在同一进程内相互竞争。

    　　分配域：系统内线程可以为其他竞争的处理器的集合。一个系统可以有一个以上的分配域，每个包含一个以上的处理器，在一个单处理器的处理机上，各个分配域可以包含从一个处理器到系统中所有的处理器。（没有实现该接口）

    1. 竞争范围内的线程可以共享一个核实体
    2. 系统竞争范围线程之间的环境切换通常要求至少一次内核调用
    3. 竞争范围在优先级调度上没有给你真正的控制——高优先级的线程可以优先于进程内的其他线程运行
    4. 当一个线程被分配超过一个处理机的分配域时，应用程序不能完全依靠可预知的调度行为；如：高优先级和低优先级可以同时运行，调度程序不允许因为一个高优先级的线程正在运行而是处理及闲置，单处理机行为在一台多处理机上没什么意思。

- 线程的调度策略：

  -  ![image-20220627152616477](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627152616477.png)
  - ![image-20220627152736317](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627152736317.png)
  - FIFO：如果优先级相同，先进先出
  - RR：如果优先级相同，抢占式调度
  - OTHER：其他情况（默认值）

- 继承调度策略

  - ![image-20220627153023707](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627153023707.png)
  - ![image-20220627153343356](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627153343356.png)
  - inherit:继承调用者的调度策略
  - explicit：不继承

- 调度参数

  - ![image-20220627153001676](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627153001676.png)
  - 只关心param中的优先级参数
  - ![image-20220627153835963](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627153835963.png)



### 并发级别

![image-20220627154128711](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627154128711.png)





### 线程中怎么弄一个线程内存共享的伪全局变量？

![image-20220627154411278](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627154411278.png)

线程自带提供128位的key来map128个value

- key创建删除

  - ![image-20220627154622555](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627154622555.png)
  - key不是自己指定的，而是通过create函数赋予的一个空闲key
  - destructor是一个析构函数，用来销毁value

- 给key设置value

  - ![image-20220627160729032](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627160729032.png)
  - value结构体自己设置

  

### 线程只执行一次

假设线程的执行函数为func  

让init_routine函数在所有线程的func中只执行一次，就可以在func中调用pthread_once

![image-20220627160820610](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627160820610.png)

## posix 信号量和锁



### 生产者与消费者模型

- 生产者：

  - ![image-20220627162634951](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627162634951.png)

  消费者

![image-20220627162725567](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627162725567.png)



### posix信号量

- sem_open：
  - 创建一个新的有名信号量或打开一个已存在的有名信号量。有名信号量既可用于线程间的同步，又可用于进程间的同步
  - sem_t *sem_open(const char *name, int oflag); //-打开
  - sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);//-初始化
  - name：信号量的名字。
    oflag：参数可以是是 0、O_CREAT、O_CREAT|O_EXCL，但如果指定了 O_CREAT 标志，那么第三个和第四个参数是需要的。
    mode：使用本参数前提是 oflag 指定了 O_CREAT 标志，mode 参数用来指定权限位。
    value：指定信号量的初始值。该初始值不能超过 SEM_VALUE_MAX（这个常值必须至少为32767）。二值信号量的初始值通常为 1，计数信号量的初始值则往往大于 1。
- sem_close：
  - int sem_close(sem_t *sem);
  - 返回值：
    成功则为 0，出错则为 -1。
    参数：
    sem：使用 sem_open 创建的信号量指针。
- sem_unlink:
  - int sem_unlink(const char *name);
  - 有名信号量使用 sem_unlink 从系统中删除。
- sem_wait(V操作)
  - sem_wait 函数测试所指定信号量的值，如果该值大于 0，那就将它减 1 并立即返回。如果该值等于0，调用线程就被投入睡眠中（阻塞 io），直到该值变为大于 0，这时再将它减 1，函数随后返回。其中“测试并减 1”这两步操作操作必须是原子的。
  - int sem_wait(sem_t *sem);
  - 返回值：成功返回 0，出错则为 -1。
- sem_post（P操作）
  - 当一个线程使用完某个信号量时，它应该调用 sem_post。把所指定信号量的值加 1，然后唤醒正在等待该信号量值变为正数的任意线程。
  - int sem_post(sem_t *sem);
  - 返回值：成功返回 0，出错返回 -1。
- sem_init
  - 创建匿名信号量（基于内存）
  - int sem_init(sem_t *sem, int pshared, unsigned int value);
  - 参数：
    sem：指向应用程序必须分配的 sem_t 变量。
    pshared：如果为 0，那么待初始化的信号量是在同一进程的各个线程间共享的，否则该信号量是在进程间共享的。当 pshared 为非零时，该信号量必须存放在某种类型的共享内存区中，而即将使用它的所有进程都要能访问该共享内存区。
    value：该信号量的初始值。
  - 返回值：出错返回 -1，成功并不一定返回 0，Posix.1 注解中说明将来的某个修订版可能指定调用成功时返回0。
- dem_destroy
  - 销毁匿名信号量
  - int sem_destroy(sem_t *sem);
  - 返回值：成功返回 0，出错返回 -1。



### 互斥锁

- pthread_mutex_init
  - pthread_mutex_init(pthread_mutex_t * mutex, const phtread_mutexattr_t * mutexattr);
  - //动态方式创建锁，相当于new动态创建一个
- pthread_mutex_destory
  - pthread_mutex_destory(pthread_mutex_t *mutex)        
  - //释放互斥锁，相当于delete
- pthread_mutex_t 
  - pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   
  - //以静态方式创建锁
- pthread_mutex_lock   
  - pthread_mutex_lock(pthread_mutex_t *mutex)           
  - //以阻塞方式运行的。如果之前mutex被加锁了，那么程序会阻塞在这里。
- pthread_mutex_unlock
  - pthread_mutex_unlock(pthread_mutex_t *mutex)		 
  - //解锁
- int pthread_mutex_trylock
  - int pthread_mutex_trylock(pthread_mutex_t * mutex);  
  - //会尝试对mutex加锁。如果mutex之前已经被锁定，返回非0；如果mutex没有被锁定，则函数返回并锁定mutex。该函数是以非阻塞方式运行了。也就是说如果mutex之前已经被锁定，函数会返回非0，程序继续往下执行。



### 自旋锁

- spin_lock_init(spinlock_t *x);  //初始化
- spin_lock(x);  					//只有在获得锁的情况下才返回，否则一直“自旋”  
- spin_trylock(x);  				//如立即获得锁则返回真，否则立即返回假  
- spin_unlock(x);  				//释放锁
- spin_is_locked(x) 				//该宏用于判断自旋锁x是否已经被某执行单元保持（即被锁），如果是，返回真，否则返回假。



### 读写锁

- int pthread_rwlock_init(pthread_rwlock_t * rwlock, const pthread_rwlockattr_t * attr);
  -  //动态读写锁初始化,该函数第一个参数为读写锁指针，第二个参数为读写锁属性指针。函数按读写锁属性对读写锁进行初始化。
- int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);	
  - //可用来销毁 rwlock 引用的读写锁对象并释放该锁使用的任何资源。
- pthread_rwlock_t  rwlock = PTHREAD_RWLOCK_INITIALIZER;  
  - //静态读写锁初始化
- int  pthread_rwlock_rdlock(pthread_rwlock_t *rwlock );	
  - //向 rwlock 所引用的读写锁加读锁。为避免写入器资源匮乏，允许在多个实现中使写入器的优先级高于读取器。
- int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);	
  - //应用读锁的方式与 pthread_rwlock_rdlock() 类似，区别在于如果任何线程持有 rwlock 中的写锁或者写入器基于 rwlock 阻塞，则 pthread_rwlock_tryrdlock() 函数会失败。
- int  pthread_rwlock_wrlock(pthread_rwlock_t *rwlock );	
  - //如果没有其他读取器线程或写入器线程持有读写锁 rwlock，则调用线程将获取写锁。否则，调用线程将阻塞。调用线程必须获取该锁之后，才能从pthread_rwlock_wrlock() 调用返回。如果在进行调用时，调用线程持有读写锁（读锁或写锁），则结果是不确定的。
- int pthread_rwlock_trywrlock(pthread_rwlock_t  *rwlock);
  - //应用写锁的方式与 pthread_rwlock_wrlock() 类似，区别在于如果任何线程当前持有用于读取和写入的 rwlock，则pthread_rwlock_trywrlock() 函数会失败。
- int pthread_rwlock_unlock (pthread_rwlock_t  *rwlock);  
  - //用来释放在 rwlock 引用的读写锁对象中持有的锁。



## 条件变量

使用信号量可以同步两个线程，但是用途还是有点狭窄了，如果业务需要的操作更复杂，仅仅对信号量的PV操作不足以表达逻辑，就只能使用条件变量。

条件变量可以指定对全局变量指定任意一个条件，在不满足条件的情况下阻塞，直到其他线程更新了全局变量，再判断是否满足条件解除阻塞。

![image-20220627215513087](C:\Users\37412\AppData\Roaming\Typora\typora-user-images\image-20220627215513087.png)

为了保证多线程对n操作的正确性，必须在操作n的时候加锁解锁。但是等待n满足条件的时候会阻塞，不会解锁，其它线程也无法更改n，会导致死锁，所以这种模型必须要用条件变量实现。



事实上生产者与消费者也可以用条件变量实现，而且buffer不像信号量实现一样，需要一个固定长度作为生产者的信号量，可以实现。

- pthread_cond_init
  - 初始化
- pthread_cond_destroy
- pthread_cond_wait
- pthread_cond_signal
- pthread_cond_broadcast
  - 广播通知



wait可能被虚假唤醒，所以需要使用while循环判定条件，额外判定虚假唤醒。











 

