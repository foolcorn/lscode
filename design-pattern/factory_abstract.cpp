// *抽象工厂模式demo
//-如果说各种工厂子类是在纵向的拓展，抽象工厂就是在工厂模式的基础上进行横向拓展
//-工厂只针对于某个对象的创建，抽象工厂则是可以封装多个对象，并根据不同的api进行调用
//-让我们重构之前的项目并进行拓展，Tony建立了复仇者大厦，将原来的IronManFactory重构成AvengeFactory
//-Tony又想到自己的亲儿子喜欢吐个丝在高楼大厦晃来荡去的所以也拉进了复仇者，构建类SpiderMan，并加入spin（吐丝）的api。
//-在英雄无归里三代同框，可以看出一代和二三代的吐丝方法是不同的，所以也用子类多态实现一下

#include<iostream>
using namespace std;

class SpiderMan{
public:
    virtual void spin() = 0;
};

//-一代蜘蛛侠
class SpiderMan1:public SpiderMan{
public:
    void spin() override{
        fprintf(stdout,"spin from body\n");
    }
};

//-二三代
class SpiderMan2_3 : public SpiderMan{
public:
    void spin() override{
        fprintf(stdout,"spin from Web Shooters\n");
    }
};


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


class AvengeFactory{
public:
    virtual IronMan* createIronMan() = 0;
    virtual SpiderMan* createSpiderMan() = 0;
    void attackEnemyByIronMan(){
        IronMan * im = createIronMan();
        im ->attackEnemy();
    }
    void spinBySpiderMan(){
        SpiderMan* sm = createSpiderMan();
        sm ->spin();
    }
};

//-如此这般，当需要组队出任务，只需要拓展一个子类工厂即可。
class M3_S1Factory:public AvengeFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark3;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
    SpiderMan* createSpiderMan() override{
        //-.....前面的准备工作可能很复杂
        SpiderMan * sm = new SpiderMan1;
        //-.....后续工作也可能很复杂
        return sm;
    }
};

class M3_S23Factory:public AvengeFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark3;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
    SpiderMan* createSpiderMan() override{
        //-.....前面的准备工作可能很复杂
        SpiderMan * sm = new SpiderMan2_3;
        //-.....后续工作也可能很复杂
        return sm;
    }
};

class M6_S1Factory:public AvengeFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark6;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
    SpiderMan* createSpiderMan() override{
        //-.....前面的准备工作可能很复杂
        SpiderMan * sm = new SpiderMan1;
        //-.....后续工作也可能很复杂
        return sm;
    }
};

class M6_S23Factory:public AvengeFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark6;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
    SpiderMan* createSpiderMan() override{
        //-.....前面的准备工作可能很复杂
        SpiderMan * sm = new SpiderMan2_3;
        //-.....后续工作也可能很复杂
        return sm;
    }
};

class M50_S1Factory:public AvengeFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark50;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
    SpiderMan* createSpiderMan() override{
        //-.....前面的准备工作可能很复杂
        SpiderMan * sm = new SpiderMan1;
        //-.....后续工作也可能很复杂
        return sm;
    }
};

class M50_S23Factory:public AvengeFactory {
public:
    IronMan* createIronMan() override{
        //-.....前面的准备工作可能很复杂
        IronMan * im = new Mark50;
        //-.....后续调试检验工作也可能很复杂
        return im;
    }
    SpiderMan* createSpiderMan() override{
        //-.....前面的准备工作可能很复杂
        SpiderMan * sm = new SpiderMan2_3;
        //-.....后续工作也可能很复杂
        return sm;
    }
};




int main(){
    string choice1;
    string choice2;
    while(true){
        cout << "input what type of IronMan, options:(m3,m6,m50,quit)"<<endl;
        cin>>choice1;
        if(choice1 == "quit"){
            break;
        }
        cout << "input what type of SpiderMan, options:(s1,s23,quit)"<<endl;
        cin>>choice2;
        if(choice1 == "m3"&&choice2 == "s1"){
            M3_S1Factory m3s1;
            m3s1.attackEnemyByIronMan();
            m3s1.spinBySpiderMan();
        }else if (choice1 == "m3"&&choice2 == "s23") {
            M3_S23Factory m3s23;
            m3s23.attackEnemyByIronMan();
            m3s23.spinBySpiderMan();
        }else if (choice1 == "m6"&&choice2 == "s1") {
            M6_S1Factory m6s1;
            m6s1.attackEnemyByIronMan();
            m6s1.spinBySpiderMan();
        }else if (choice1 == "m6"&&choice2 == "s23") {
            M6_S23Factory m6s23;
            m6s23.attackEnemyByIronMan();
            m6s23.spinBySpiderMan();
        }else if (choice1 == "m50"&&choice2 == "s1") {
            M50_S1Factory m50s1;
            m50s1.attackEnemyByIronMan();
            m50s1.spinBySpiderMan();
        }else if (choice1 == "m50"&&choice2 == "s23") {
            M50_S23Factory m50s23;
            m50s23.attackEnemyByIronMan();
            m50s23.spinBySpiderMan();
        }else if(choice2 == "quit"){
            break;
        }
    }

    exit(EXIT_SUCCESS);
}


