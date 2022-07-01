// *观察者模式demo
//-定义一个一对多的关系，当一个对象状态发生改变的时候，依赖的所有对象都得到通知，并自动更新。
//-比如多个账号订阅了某个公众号的文章，每当公众号更新了新的文章，就会把文章推送到每个账号里。
//-每个账号则会根据终端的不同（pc,手机app，微信小程序等）有不同的提示消息。

#include <string>
#include <iostream>
#include <set>
using namespace std;

//-普通账号
class normal_account{
public:
    virtual void showMessage(string message){}
};

//-电脑端
class normal_account_pc : public normal_account{
public:
    void showMessage(string message) override{
        cout<<"pc show:"<<message<<endl;
    }
};

//-手机端
class normal_account_phone : public normal_account{
public:
    void showMessage(string message) override{
        cout<<"phone show:"<<message<<endl;
    }
};

//-公众号
class public_account{
private:
    set<normal_account*> account_list;
    virtual string getNewMessage(){
        return "new message xxx";
    }
public:
    //-订阅
    void attach(normal_account* na);
    //-取消订阅
    void detach(normal_account* na);
    //-通知新msg
    void notify();
};

void public_account::attach(normal_account *na) {
    if(account_list.count(na)>0){
        return;
    }
    account_list.insert(na);
}

void public_account::detach(normal_account *na) {
    if(account_list.count(na) == 0){
        return;
    }
    account_list.erase(na);
}

void public_account::notify() {
    string msg = getNewMessage();
    for(auto it = account_list.begin(); it != account_list.end(); ++it){
        (*it)->showMessage(msg);
    }
}

int main(){
    //-pc终端用户
    normal_account_pc napc;
    //-手机终端用户
    normal_account_phone naph;
    //-公众号
    public_account pa;
    //-订阅
    pa.attach(&napc);
    pa.attach(&naph);
    //-通知
    pa.notify();
    //-取消订阅
    pa.detach(&naph);
    //-通知
    pa.notify();
    exit(EXIT_SUCCESS);
}