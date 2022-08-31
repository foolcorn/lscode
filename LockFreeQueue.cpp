<<<<<<< HEAD
//-多写多读的无锁队列


=======
// *基于zeroMQ的改写的单写单读无锁队列，简化易读
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;
//-实现一个提供原子操作的指针类型
template<typename T>
class atomic_ptr{
private:
    volatile T* ptr = nullptr;
public:
    //-cas函数,比较ptr和cmp,如果相同,就赋予ptr新值new_ptr，并返回旧值。
    T* atomic_cas(T* cmp, T* new_ptr){
        int *old_ptr;
        __asm__ volatile ("lock; cmpxchg %2, %3"
        : "=a" (old_ptr), "=m" (ptr)
        : "r" (new_ptr), "m" (ptr), "0" (cmp)
        : "cc", "memory");
        return old_ptr;
    }
    //-atomic_set,设置ptr为为新值。返回ptr旧值
    T* atomic_set(T* new_ptr){
        T* old;
        __asm__ volatile ("lock; xchg %0, %2"
                             : "=r" (old), "=m" (ptr)
                             : "m" (ptr), "0"(new_ptr));
        return old;
    }
    //-非原子set
    void set(T* new_ptr){
        ptr = new_ptr;
    }
    //-禁止移动和拷贝
    atomic_ptr() = default;
    atomic_ptr(const atomic_ptr&) = delete;
    const atomic_ptr& operator=(const atomic_ptr&) = delete;
    atomic_ptr(atomic_ptr &&) = delete;
    const atomic_ptr & operator = (atomic_ptr && ) = delete;
};


//-实现无锁队列的存储数据结构
//-第一层看，是一个双向链表
//-第二层看，每个链表节点中有数组 buffer_length-每个节点内数组的长度
template<typename T, int buffer_length>
class storage_model{
private:
    //-私有内部类
    struct storage_node{
        T buffer[buffer_length];
        storage_node * prev;
        storage_node * next;
    };
    //-第一个元素所在的节点（链表头结点，即便整个队列没有元素该节点也会存在）
    storage_node *begin_node;
    //-当前队列中第一个元素所在的begin_node中的buffer的下标
    int begin_index;

    //-最后一个元素所在的节点（并不一定是当前链表的尾结点）
    storage_node *last_node;
    //-当前队列中最后一个元素所在的last_node中的buffer的下标
    int last_index;

    //-链表尾结点，可能只是一个malloc后无元素的空节点
    storage_node *end_node;
    //-最后一个malloc的元素在end_node中的buffer下标
    int end_index;

    //-假设队列缩减容量，需要释放空的node，会以这个原子指针来存一个最新free掉的node，
    //-预留给下次malloc的时候备用
    atomic_ptr<storage_node> new_free_node;

public:
    //-构造函数
    storage_model():
    begin_node((storage_node*)calloc(1,sizeof(storage_node))),//-初始就有一个node
    begin_index(0),
    last_node(nullptr),//-lastnode是最后一个元素所在的node，没有元素，所以为空
    last_index(0),
    end_node(begin_node),//-链表尾即链表头
    end_index(0)
    {};

    //-尾部插入元素
    void push_back(const T& ele){
        //-将新元素插入到尾部预留的T元素大小的空间
        last_node = end_node;
        last_index = end_index;
        last_node->buffer[last_index] = ele;
        //-给下一个插入的元素预留空间
        malloc_back();
    }

    //-给链表last_index后追加(预留)一个T元素的空间
    void malloc_back(){
        if(++end_index < buffer_length){
            return;
        }
        //-需要新的node
        //-返回free node的元素，看看有没有空闲的node
        storage_node * temp = new_free_node.atomic_set(nullptr);
        if(!temp){//-如果没有free node，只能重新分配
            temp = (storage_node*)calloc(1,sizeof(storage_node));
            if(!temp){
                perror("calloc");
                exit(EXIT_FAILURE);
            }
        }
        end_index = 0;
        end_node->next = temp;
        temp ->prev = end_node;
        temp ->next = nullptr;
        end_node = temp;
    }

    //-尾部弹出元素（实际上改last_node和last_index的指向就行）
    void pop_back(){
        if(--last_index == -1){
            //-如果弹出该元素后，node的buffer清空
            last_index = buffer_length - 1;
            last_node = last_node->prev;
        }
        //-回收预留的T元素大小的空间
        free_back();
    }

    //-回收预留的T元素大小的空间
    void free_back(){
        if(--end_index == -1){//-如果删除了预留的空间，end_node的buffer清空
            end_index = buffer_length -1;
            end_node = end_node->prev;
            storage_node * old_free = new_free_node.atomic_set(end_node->next);//-将需要清空的节点更新到new_free_node
            if(old_free){//-真正free掉旧的
                free(old_free);
            }
            end_node ->next = nullptr;
        }
    }

    //-头部弹出元素（由于不用实现头部插入，所以永远不需要给头部预留T空间，pop头部原理和free_back类似）
    //-(实际上改begin_node和begin_index的指向就行)
    void pop_front(){
        if(++begin_index == buffer_length){
            begin_index = 0;
            begin_node = begin_node->next;
            storage_node * old_free = new_free_node.atomic_set(begin_node->prev);
            if(old_free){
                free(old_free);
            }
            begin_node -> prev = nullptr;
        }
    }

    //-获取第一个元素引用
    T& front(){
        return begin_node->buffer[begin_index];
    }

    //-获取最后一个元素的引用
    T& back(){
        return last_node->buffer[last_index];
    }

    //-返回预留的插入位置
    T& end() {
        return end_node -> buffer[end_index];
    }

    //-析构函数
    ~storage_model(){
        //-顺着node链表free
        while(begin_node!=end_node){
            last_node = begin_node;//-析构的时候last_node就没用了，拿来当个临时指针用而已
            begin_node = begin_node->next;
            free(last_node);
        }
        free(end_node);
        //-如果有free_node也删除
        last_node = new_free_node.atomic_set(nullptr);
        if(last_node){
            free(last_node);
        }
    };

    //-禁止拷贝和移动
    storage_model(const storage_model&) = delete;
    const storage_model & operator = (const storage_model &) = delete;
    storage_model(storage_model &&) = delete;
    const storage_model & operator = (storage_model && ) = delete;
};

//-最上层适配器，无锁队列
template<typename T, int buffer_length>
class lock_free_queue{
private:
    //-复合之前定义的存储结构,表层是个双向链表，所以叫它list
    storage_model<T,buffer_length> sm_list;
    T* producer_end = nullptr;//-对于生产者可见的队列中最后一个元素
    T* consumer_end = nullptr;//-对于消费者可见的队列中的最后一个元素
    T* accumulate_head = nullptr;//-指向累计缓存（未更新）的第一个元素，如果该指针更新，意味着缓存的元素全部更新到队列里
    atomic_ptr<T> sign;//-一个标志位，主要用来标志消费者是否读完队列所有元素，

public:
    //-构造
    lock_free_queue(){
        //-先预留一个插入位置
        sm_list.malloc_back();
        accumulate_head = &sm_list.end();
        sign.set(nullptr);
        producer_end = accumulate_head;
        consumer_end = accumulate_head;

    }

    //-向队列写数据(支持缓冲追加),如果complete==false则只会累计在缓存里，不会真的更新队列
    //-若complete == true则立刻把所有累计的一起更新
    void write(const T& ele,bool complete){
        sm_list.push_back(ele);
        if(complete){
            accumulate_head = &sm_list.end();
        }
    }

    //-删除累计缓存的最后一个数据，循环调用此方法可以清空所有数据
    bool clear_last_push_buffer(){
        //-如果缓存没数据，就直接返回
        if(accumulate_head == &sm_list.end()){
            return false;
        }
        else{
            sm_list.pop_back();
            return true;
        }
    }

    //-更新producer_end（生产者可见队列终点）,而返回值代表消费者是否之前读完了队列中所有元素。
    bool updateProducerEnd(){
        //-如果标志位的旧值没有变，意味着消费者线程之前没读完，依然在运行
        if(sign.atomic_cas(producer_end,accumulate_head) == producer_end){
            //-更新可读元素结尾
            producer_end = accumulate_head;
            return false;
        }else{
            //-如果旧值!=read_end,只可能是被消费者读完所有元素后改成nullptr了，
            sign.set(accumulate_head);
            producer_end = accumulate_head;
            return true;//-反映出之前消费者是否读完了数据，若为true，则后续可以通知生产者给其发信号告知有新元素可读了
        }
    }


    //-判断当前是否可读
    bool checkRead(){
        if(&sm_list.front() != consumer_end && consumer_end){
            return true;//-队列中还有元素没读完，返回true
        }
        //-front走到了accumulate_head的位置，意味着读完了，设置sign为nullptr
        consumer_end = sign.atomic_cas(&sm_list.front(),nullptr);
        if(consumer_end == &sm_list.front() || !consumer_end){
            return false;
        }
        //-如果生产者又生产了新的元素
        return true;
    }

    bool read(T& result){
        if(!checkRead()){
            return false;
        }
        result = sm_list.front();
        sm_list.pop_front();
        return true;
    }
};

//-生产者快，消费者慢
void* work_thread_func(void* arg){
    lock_free_queue<int,1> *lfqptr = (lock_free_queue<int,1>*) arg;
    for (int i = 0; i < 100;++i) {
        lfqptr->write(i,true);
        cout<<"write "<<i<<" to queue"<<endl;
        if(lfqptr -> updateProducerEnd()){
            cout<<"log threads read all"<<endl;
        }
        usleep(200);
    }
    return nullptr;
}

void* log_thread_func(void* arg){
    lock_free_queue<int,1> *lfqptr = (lock_free_queue<int,1>*) arg;
    while(true){
        int temp;
        if(lfqptr ->read(temp)){
            cout<<"read "<< temp<<" from queue"<<endl;
        }
        usleep(500);
    }
    return nullptr;
}

int main() {
    //-定义一个无锁队列
    lock_free_queue<int,1> lfq;
    pthread_t tid1;
    pthread_t tid2;
    pthread_create(&tid1, NULL,work_thread_func,&lfq);
    pthread_create(&tid2, NULL,log_thread_func,&lfq);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}

>>>>>>> bea49a95b74a504d965db454435bc1277334674c
