/*
 * 手撕智能指针：
 * 本身保证异常安全
 * 不保证线程安全：如果多线程读写共享的sharedptr需要加锁
 * 为什么：因为shareptr的赋值指针，和更改引用计数是两个操作而非原子，在多线程下就会出现问题。
 * 假设三个智能指针S，A，B      S为共享shareptr，AB为各自的局部shared_ptr
 * 此时线程1若用S赋值给A,刚赋值完ptr，就切换了线程2，此时因为usecount默认是0，所以该对象会被shareptr销毁
 * 线程2中，用B赋值给S，S的ptr=B的ptr，同时更新S的引用计数为2，
 * 切回线程1，usecount更新为S的usecuont+1，SAB的usecount都变成了3，但是A的实际对象却已经不存在，而SB的usecount更是错误增加1。
 */

#include <memory>
#include <iostream>
#include <atomic>
using namespace std;

namespace fc{
    //-定义一个callable对象类作为默认删除器
    template<typename T>
    class deafult_deleter{
    public:
        void operator()(T*ptr){
            delete ptr;
        }
    };

    template<typename T,typename deleter = deafult_deleter<T>>
    class shared_ptr{
    private:
        //-原生堆指针
        T*ptr = nullptr;
        //-指向use_count的指针，事实上use_count本是更复杂的结构体其中一个成员，所以用指针保留拓展性
        std::atomic<int> *use_count = nullptr;
    public:
        //-默认构造
        shared_ptr() = default;
        //-原生指针构造
        shared_ptr(T*ptr_);
        //-拷贝构造
        shared_ptr(const shared_ptr&lsh);
        //-移动构造
        shared_ptr(shared_ptr && lsh);
        //-拷贝赋值运算符
        shared_ptr& operator=(const shared_ptr& lsh);
        //-移动赋值运算符
        shared_ptr & operator = (shared_ptr && lsh);
        //-析构
        ~shared_ptr();

        //-箭头函数运算符重载
        T* operator->();
        //-解引用运算符重载
        T& operator*();

        //-交换
        void swap(shared_ptr & lsh);
        //-解除对当前指针的管理, 如果有新的ptr则托管新的指针
        void reset(T*ptr_ = nullptr);
        //-获取引用计数
        int count();
        //-获取原生指针
        T* get();
    };

    //-原生指针构造
    template<typename T, typename deleter>
    shared_ptr<T, deleter>::shared_ptr(T *ptr_)
    :ptr(ptr_),use_count(new std::atomic<int>(1)){}

    //-拷贝构造
    template<typename T, typename deleter>
    shared_ptr<T, deleter>::shared_ptr(const shared_ptr &lsh) {
        ptr = lsh.ptr;
        use_count = lsh.use_count;
        ++*use_count;
    }

    //-移动构造
    template<typename T, typename deleter>
    shared_ptr<T, deleter>::shared_ptr(shared_ptr &&lsh) {
        std::swap(ptr,lsh.ptr);
        std::swap(use_count, lsh.use_count);
    }

    //-拷贝赋值运算符
    template<typename T, typename deleter>
    shared_ptr<T, deleter> &shared_ptr<T, deleter>::operator=(const shared_ptr &lsh) {
        shared_ptr<T, deleter>lsh_copy(lsh);
        swap(lsh_copy);
        return *this;
    }

    //-移动赋值运算符
    template<typename T, typename deleter>
    shared_ptr<T, deleter> &shared_ptr<T, deleter>::operator=(shared_ptr &&lsh) {
        if (this != & lsh) {//-另一种方式实现自赋值
            //-如果原来有托管对象
            if(ptr){
                --*use_count;
                if(*use_count <=0){
                    deleter()(ptr);
                    delete use_count;
                }
                ptr = nullptr;
                use_count = nullptr;
            }
            swap(lsh);
        }
        return *this;
    }

    //-析构
    template<typename T, typename deleter>
    shared_ptr<T, deleter>::~shared_ptr() {
        if(ptr){
            --*use_count;
            if(*use_count<=0){
                deleter()(ptr);
                delete use_count;
            }
        }
    }

    //-箭头函数运算符重载
    template<typename T, typename deleter>
    T *shared_ptr<T, deleter>::operator->() {
        return ptr;
    }

    //-解引用运算符重载
    template<typename T, typename deleter>
    T &shared_ptr<T, deleter>::operator*() {
        return *ptr;
    }

    //-交换
    template<typename T, typename deleter>
    void shared_ptr<T, deleter>::swap(shared_ptr &lsh) {
        std::swap(ptr, lsh.ptr);
        std::swap(use_count,lsh.use_count);
    }

    //-解除对当前指针的管理, 如果有新的ptr则托管新的指针
    template<typename T, typename deleter>
    void shared_ptr<T, deleter>::reset(T *ptr_) {
        if(ptr){//-如果当前shared_ptr有托管对象，先释放原托管
            --*use_count;
            if(*use_count <= 0){
                deleter()(ptr);
                delete use_count;
            }
            ptr = nullptr;
            use_count = nullptr;
        }
        //-是否有新托管
        if(ptr_){
            ptr = ptr_;
            use_count = new std::atomic<int>(1);
        }
    }

    //-获取引用计数
    template<typename T, typename deleter>
    int shared_ptr<T, deleter>::count() {
        return *use_count;
    }

    //-获取原生指针
    template<typename T, typename deleter>
    T *shared_ptr<T, deleter>::get() {
        return ptr;
    }
}

//-测试类
class Student{
public:
    int id;
    int age;
    Student(int id_, int age_):id(id_), age(age_){}
};

void print(fc::shared_ptr<Student> &s){
    if(s.get() == nullptr){
        cout<<"nullptr"<<endl;
        return;
    }
    cout<<"id:"<<s->id<<"  "<<"age:"<<s->age<<endl;
    cout<<"use count:"<< s.count() << endl;
}
int main(int argc, char *argv[]){
    {
        cout<<"拷贝构造测试，解引用，箭头函数测试"<<endl;
        fc::shared_ptr<Student> s1(new Student(0,18));
        fc::shared_ptr<Student> s2(s1);
        (*s1).age = 19;
        print(s2);
        cout<<"自赋值测试"<<endl;
        s1 = s1;
        print(s1);
    }

    {
        cout<<"测试reset"<<endl;
        fc::shared_ptr<Student> s1(new Student(0,18));
        s1.reset();
        print(s1);
        s1.reset(new Student(1,19));
        print(s1);

    }
    {
        cout<<"移动构造测试"<<endl;
        fc::shared_ptr<Student> s1(fc::shared_ptr<Student>(new Student(0,18)));
        print(s1);

        cout<<"拷贝赋值运算符测试"<<endl;
        fc::shared_ptr<Student> s2;
        s2 = s1;
        print(s1);
        print(s2);

        cout<<"移动赋值运算符测试"<<endl;
        fc::shared_ptr<Student> s3 (new Student(2,20));
        s1 = std::move(s3);
        print(s1);
        print(s2);
        print(s3);
    }
    return 0;
}