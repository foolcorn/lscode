// *工厂模式demo1
//-假设Tony stark想要攻击敌人，需要制作钢铁战衣，而且因为战衣的种类不同，攻击方式也不同，可以用子类多态实现
//-mark3的手臂附有小型导弹
//-mark6的手臂上有镭射激光
//-mark50有纳米激光炮
//-我们可以考虑用工厂模式封装战衣的制作过程，对应于战衣的多个子类，我们可以设计多个对应的子类工厂

#include<iostream>
using namespace std;

class IronMan{
public:
    virtual void attackEnemy() = 0;
};

class Mark3:public IronMan{
public:
    void attackEnemy() override{
        fprintf(stdout,"attack enemy by little missile\n");
    }
};

class Mark6:public IronMan{
public:
    void attackEnemy() override{
        fprintf(stdout,"attack enemy by laser\n");
    }
};

class Mark50:public IronMan{
public:
    void attackEnemy() override{
        fprintf(stdout,"attack enemy by nanotechnology laser cannon\n");
    }
};

class IronManFactory{
public:
    virtual IronMan* createIronMan() = 0;
};

class Mark3Factory:public IronManFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark3;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
};

class Mark6Factory:public IronManFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark6;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
};

class Mark50Factory:public IronManFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark50;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
};

int main(){
    string choice;
    while(true){
        cout << "input what type of IronMan, options:(mark3,mark6,mark50,quit)"<<endl;
        cin>>choice;
        if(choice == "mark3"){
            Mark3Factory m3f;
            IronMan * im = m3f.createIronMan();
            im->attackEnemy();
        }else if (choice == "mark6") {
            Mark6Factory m6f;
            IronMan * im = m6f.createIronMan();
            im->attackEnemy();
        }else if (choice == "mark50") {
            Mark50Factory m50f;
            IronMan * im = m50f.createIronMan();
            im->attackEnemy();
        }else if(choice == "quit"){
            break;
        }
    }

    exit(EXIT_SUCCESS);
}