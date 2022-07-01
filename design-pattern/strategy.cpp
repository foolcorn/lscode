// *策略模式demo
//-策略模式的思路是如果一个算法有很多ifelse结构，可以考虑将每种分支的算法抽象成一个类
//-和模板方法的区别：模板方法主类已经框定了整个算法框架，只是内部每个步骤的实现有不同
//-策略模式则是根据不同情况选择不同的算法实现类作为参数传进主类。

//-比如给定一个学生的百分制分数，将其转换成其他表示形式，有很多种计算方法，且不同计算方法之间是平行相互独立的。
//-十分制，ABC级别，优良差等等，就可以用策略模式

#include <iostream>
#include <string>
using namespace std;

class Strategy{
public:
    virtual string calc(int score)=0;
};

//-十分制
class TenStrategy:public Strategy{
public:
    string calc(int score) override{
        return to_string(score/10);
    }
};

//-ABC等级
class LevelStrategy : public Strategy{
public:
    string calc(int score) override {
        if(score >=90){
            return "A";
        }else if(score>=80){
            return "B";
        }else if(score >=60){
            return "C";
        }else{
            return "D";
        }
    }
};

class ShowScore{
private:
    Strategy * strategy;
public:
    ShowScore(Strategy * strategy_):strategy(strategy_){}
    void print(int score){
        string res = strategy -> calc(score);
        cout <<"result: "<<res<<endl;
    }
};


int main(){
    int score = 85;
    //-定义策略
    TenStrategy ts;
    LevelStrategy ls;
    //-两种策略分别打印
    ShowScore ss_ts(&ts);
    ss_ts.print(score);
    ShowScore ss_ls(&ls);
    ss_ls.print(score);
    exit(EXIT_SUCCESS);
}



