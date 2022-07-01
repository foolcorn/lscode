// *单例+懒汉+线程安全
class singleton{
private:
    singleton();//-将构造函数设为私有，这样用户无法自己创建对象
public:
    //-将拷贝构造，移动构造，拷贝运算符，移动运算符删除
    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;
    singleton(singleton &&) = delete;
    singleton & operator = (singleton && ) = delete;
    //-public析构，当进程结束后，内存销毁静态局部变量时会调用析构
    ~singleton();

    //-只适合c++11以上标准
    static singleton& getInstance(){
        //-静态局部变量实现懒汉模式，只有第一次调用getInstance的时候才会初始化instance，且全局只会初始化一次
        //-c++11以上标准中，静态局部变量初始化是线程安全的，当其中一个线程初始化instance的时候，会阻塞其它线程的初始化行为
        static singleton instance;
        return instance;
    }
};

#include <mutex>
#include <atomic>
//-另一个种方式
class singleton_old{
private:
    static std::atomic<singleton_old*> instance_;//-静态实例指针
    static std::mutex mutex_;//-互斥锁
    static void Destroy(){
        singleton_old * temp = instance_.load(std::memory_order_relaxed);
        if(temp!=nullptr){
            delete temp;
        }
    }
    singleton_old(){}
public:
    singleton_old(const singleton_old &)=delete;
    singleton_old & operator = () = delete;
    singleton(singleton &&) = delete;
    singleton & operator = (singleton && ) = delete;
    static singleton_old * getInstance(){
        //-宽松序获取当前instance（无视顺序重排，即便该语句被重排到fence之后也没关系，因为最多会被重排到if语句之前）
        singleton_old * temp = instance_.load(std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_acquire);//-设置acquire内存屏障，意味着以下读写不会被重排到acquire之前
        if(temp == nullptr){
            //-等待拿到互斥锁
            std::lock_guard <std::mutex> lock(mutex);
            //-需要再次判断其它线程是否已经修改了instance
            temp = instance_.load(std::memory_order_relaxed);
            if(temp == nullptr){
                //-new一个新的instance
                temp = new singleton_old;
                //-解除内存屏障
                std::atomic_thread_fence(std::memory_order_release);//-当前线程读写不会被重排到release之后
                //-将新的值写入到instance_里
                instance_.store(temp,std::memory_order_relaxed);//-重拍到
                //-注册析构函数
                atexit(Destroy);
            }
        }
        return temp;
    }
};
//-静态成员需要在类外初始化
singleton_old::instance_;
singleton_old::mutex_;

int main(){

}