#include<iostream>
#include<thread>
using namespace std;

class Singleton {
public:
    static Singleton * GetInstance() {
        return instance;
    }
    int a = 0;
private:
    Singleton(){}//构造
    Singleton(const Singleton &clone){} //拷⻉构造
    Singleton& operator=(const Singleton&) {}
    static Singleton * instance;
};
Singleton* Singleton::instance = new Singleton();//静态成员需要初始化

int main(){
    Singleton* s = Singleton::GetInstance();
    cout<<s->a<<endl;

    return 0;
}