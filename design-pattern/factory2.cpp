// *工厂模式demo2
//-让我们回归初心，Tony stark的最初的目的只是想要攻击敌人，而非制作钢铁战衣（战衣只是一种方式）
//-在第三部电影中，tony生产了很多装甲，而且每个战衣可以自动作战，而他自己只需要远程遥控就可以消灭对手。
//-既然如此，我们可以将create和attack两个动作抽象并封装在一起成为独立的api attackEnemyByIronMan。

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
    void attackEnemyByIronMan(){
        IronMan * im = createIronMan();
        im ->attackEnemy();
    }
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
            m3f.attackEnemyByIronMan();
        }else if (choice == "mark6") {
            Mark6Factory m6f;
            m6f.attackEnemyByIronMan();
        }else if (choice == "mark50") {
            Mark50Factory m50f;
            m50f.attackEnemyByIronMan();
        }else if(choice == "quit"){
            break;
        }
    }

    exit(EXIT_SUCCESS);
}