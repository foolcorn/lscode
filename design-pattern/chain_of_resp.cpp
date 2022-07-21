// *责任链模式demo
//-类似请假场景，如果一天以内，可以直接向系统申请，系统自动批假
//-如果三天以内，则需要向部门经理请假
//-三天以上，则需要老板来批假

#include <iostream>
#include <string>
using namespace std;

//-假期表单
class VocationContext{
public:
    string name;//-请假人
    int days;//-天数
    VocationContext(string name_,int days_):name(name_), days(days_) {}
};

class HandlerClass{
private:
    HandlerClass * next_handler = nullptr;//-存储责任链的下一个责任人是核心
    //-自己是否能处理申请
    virtual bool canHandle(VocationContext vc) = 0;
public:
    //-设置下一链
    void setNextHandler(HandlerClass * hc){
        next_handler = hc;
    }
    //-是否能获取下一链
    HandlerClass * getNextHandler(){
        return next_handler;//-只是用作判断条件
    }
    //-处理请求
    bool handle(VocationContext vc){
        if(canHandle(vc)){
            return handleVocation(vc);
        }else if(getNextHandler()){
            return next_handler->handle(vc);
        }else {
            //-其它错误
            return false;
        }
    };
    //-是否批假(子类各自的业务决定)
    virtual bool handleVocation(VocationContext vc) = 0;
};

class SystemHanlder:public HandlerClass{
private:
    bool canHandle(VocationContext vc) override{
        return vc.days<=1;
    }
public:
    bool handleVocation(VocationContext vc) override{
        //-业务决定是否要通过
        bool res = rand()%10>2;
        cout<<"system handle result:"<< (res?"yes":"no")<<endl;
        return res;
    }
};

class ManagerHanlder:public HandlerClass{
private:
    bool canHandle(VocationContext vc) override{
        return vc.days<=3;
    }
public:
    bool handleVocation(VocationContext vc) override{
        //-业务决定是否要通过
        bool res = rand()%10>3;
        cout<<"manager handle result:"<<(res?"yes":"no")<<endl;
        return res;
    }
};

class BossHanlder:public HandlerClass{
private:
    bool canHandle(VocationContext vc) override{
        return true;
    }
public:
    bool handleVocation(VocationContext vc) override{
        //-业务决定是否要通过
        bool res = rand()%10>4;
        cout<<"boss handle result:"<<(res?"yes":"no")<<endl;
        return res;
    }
};


//-员工
class Woker{
private:
    string name;
    HandlerClass*hc;//-默认处理第一链
public:
    void applyVacaion(int days){
        VocationContext vc(name,days);
        hc->handle(vc);
    }
    Woker(string name_,HandlerClass * hc_):name(name_),hc(hc_){}
};

int main(){
    //-构建链条
    HandlerClass* sh = new SystemHanlder;
    HandlerClass* mh = new ManagerHanlder;
    HandlerClass* bh = new BossHanlder;
    sh->setNextHandler(mh);
    mh -> setNextHandler(bh);

    //-员工
    Woker w("tony",sh);

    //-请假
    w.applyVacaion(1);
    w.applyVacaion(1);
    w.applyVacaion(1);
    w.applyVacaion(1);
    w.applyVacaion(1);
    w.applyVacaion(2);
    w.applyVacaion(2);
    w.applyVacaion(2);
    w.applyVacaion(2);
    w.applyVacaion(5);
    w.applyVacaion(5);
    w.applyVacaion(5);
    w.applyVacaion(5);
    w.applyVacaion(5);
    w.applyVacaion(5);

    return EXIT_SUCCESS;

}

