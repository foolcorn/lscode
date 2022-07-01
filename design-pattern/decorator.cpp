// *装饰器模式demo
//-事实上装饰器模式也可以看做是一条链，在结构上和责任链很像
//-但是在概念上还是有点区别，
//-责任链是让一个对象在链中传递直到链中某个元素处理该对象，所以传递该对象需要链头作为传递的开始。
//-装饰器的概念则是，如果一个对象本身就具备一定的完备功能，但是这个对象在链中传递可以不断拓展自己的功能，
//-所以最后取链尾作为拓展完功能的结果。

//-据钢铁侠的例子，
//-mark1是第一代，只有一定的基础功能。
//-mark2增加了稳定飞行功能
//-mark6增加了新的能源
//-mark50增加了纳米技术

#include<iostream>
#include<string>
using namespace std;


class iron_man{
private:
    iron_man* base_type;
public:
    iron_man(iron_man* base_type = nullptr):base_type(base_type){}
    virtual string feature(){
        return "base feature";
    }
};

class mark2:public iron_man{
private:
    iron_man* base_type;
public:
    mark2(iron_man* base_type):base_type(base_type){}
    string feature() override{
        return base_type->feature()+" + stable flying";
    }
};

class mark6:public iron_man{
private:
    iron_man* base_type;
public:
    mark6(iron_man* base_type):base_type(base_type){}
    string feature() override{
        return base_type->feature()+" + new power";
    }
};

class mark50:public iron_man{
private:
    iron_man* base_type;
public:
    mark50(iron_man* base_type):base_type(base_type){}
    string feature() override{
        return base_type->feature()+" + nanotechnology";
    }
};

int main(){
    iron_man* m1 = new iron_man;
    iron_man * m2 = new mark2(m1);
    iron_man * m6 = new mark6(m2);
    iron_man * m50 = new mark50(m6);

    cout<<m1 -> feature()<<endl;
    cout<<m2 -> feature()<<endl;
    cout<<m6 -> feature()<<endl;
    cout<<m50 -> feature()<<endl;
    return EXIT_SUCCESS;
}