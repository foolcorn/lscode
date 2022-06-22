#include <iostream>
#include <deque>
#include <iterator>
#include <unordered_set>
#include <algorithm>
using KEY_TYPE = int;

class Btree_node{
    public:
    std::deque<KEY_TYPE> keys;
    std::deque<Btree_node*> children;
    bool isleaf;//-当前是否是叶子节点
    Btree_node(bool isleaf_):isleaf(isleaf_){}
};

class Btree{
private:
    Btree_node* root;//-根节点
    int M;//-B树的阶
public:
    Btree(int M_):M(M_){
        root = new Btree_node(true);
        //-可以想象这个nullptr永远是children的最后一个元素，children的size永远比keys大一位
        root->children.push_back(nullptr);
    }
    //-插入key
    void insertKey(KEY_TYPE key);

    //-当前节点的某个孩子节点（index）满了，分裂该孩子
    void splitChildren(Btree_node* node, int index);

    //-当前节点cur非空的情况下，递归查找合适的位置插入key
    void insertNonFull(Btree_node* cur, KEY_TYPE key);

    //-删除key
    void deleteKey(KEY_TYPE key);

    //-从当前节点递归查找要删除的元素
    void deleteTrace(Btree_node* cur, KEY_TYPE key);

    //-层序打印B树
    void print();

};

//-插入key
void Btree::insertKey(KEY_TYPE key) {
    //-先判断root有没有满
    if(root->keys.size() == M-1){
        //-创建新的节点（非叶子）
        Btree_node * new_node = new Btree_node(false);
        //-让root成为新节点的孩子，并复用下面的splitChildren代码
        new_node->children.push_front(root);
        splitChildren(new_node,0);
        root = new_node;

    }
    //-从root开始递归找适合key插入的位置
    insertNonFull(root,key);
}

//-当前节点的某个孩子节点（index）满了，分裂该孩子
void Btree::splitChildren(Btree_node* cur,int index){
    Btree_node * child = cur -> children[index];

    //-上浮孩子的中间元素到上层节点
    cur->keys.insert(cur->keys.begin()+index,child->keys[(M-1)/2]);

    //-新建一个节点存储child的右半部分
    Btree_node * child_right = new Btree_node(child->isleaf);
    //-将右半部分元素移动过去
    for(int i = 0;i<((M-1)/2);++i){
        child_right->keys.push_front(child -> keys.back());
        child -> keys.pop_back();
    }
    //-pop掉中间元素
    child->keys.pop_back();
    //-分裂child的孩子
    for (int i = 0; i < (M / 2); ++i) {
        child_right->children.push_front(child -> children.back());
        child -> children.pop_back();
    }
    //-由于上浮了元素到cur，所以cur的children也需要改变,插入child_right
    cur->children.insert(cur -> children.begin()+index+1,child_right);
}

//-当前节点cur非空的情况下，递归查找合适的位置插入key
void Btree::insertNonFull(Btree_node* cur, KEY_TYPE key){
    //-递归终点，当递归到叶子节点时，叶子结点必不满，因为在上层就会判断是否要分裂
    if(cur -> isleaf){
        int i = 0;
        for(; i<cur->keys.size();++i){
            if(key<=cur -> keys[i]){
                break;
            }
        }
        //-如果是重复key就放弃插入
        if(key == cur -> keys[i]){
            return;
        }
        cur->keys.insert(cur->keys.begin()+i,key);
        //-添加keys的同时,给children也同index添加一个空指针，children[i]代表比keys[i]小的节点结合
        cur->children.insert(cur -> children.begin()+i, nullptr);

        return;
    }

    //-非叶子节点，根据keys的index找到对应插入的children位置
    int i = 0;
    for(;i<cur -> keys.size(); ++i){
        if(key<=cur->keys[i]){
            break;
        }
    }
    //-如果是重复key就放弃插入
    if(key == cur -> keys[i]){
        return;
    }
    //-判断children有没有满,如果满了，分裂孩子
    if(cur->children[i]->keys.size()==M-1){
        splitChildren(cur,i);
        //-分裂后，此时cur的keys有新元素上浮，需要重新判断插入位置i
        if(key>cur -> keys[i]){
            ++i;
        }
    }
    //-在孩子处自递归直到叶子结点
    insertNonFull(cur-> children[i],key);

}

//-删除元素
void Btree::deleteKey(KEY_TYPE key){
    //-如果root没有元素，return
    if(root->keys.empty()){
        return;
    }
    deleteTrace(root,key);
}

//-从当前节点递归查找要删除的元素
void Btree::deleteTrace(Btree_node* cur, KEY_TYPE key){
    //-如果cur是叶子节点，直接遍历所有元素
    if(cur->isleaf){
        int i = 0;
        for(;i<cur -> keys.size(); ++i){
            if(key == cur -> keys[i]){
                break;
            }
        }
        //-如果在叶子节点都查找不到，说明元素不在B树中
        if(i == cur->keys.size()){
            std::cout<<"this key not exist in tree"<<std::endl;
            return;
        }
        //-叶子结点直接删除元素
        cur -> keys.erase(cur -> keys.begin()+i);
        return;
    }
    //-如果cur是非叶子节点
    int i = 0;
    for(;i<cur -> keys.size(); ++i){
        if(key <= cur -> keys[i]){
            break;
        }
    }
    //-如果cur找不到删除元素，且key可能在最右侧的孩子里
    if(i == cur -> keys.size()){
        //-获取左右孩子
        --i;
        Btree_node*left = cur -> children[i];
        Btree_node * right = cur -> children[i+1];
        if(right->keys.size() == (M-1)/2){//-右孩子可能存在删除节点后keys数量不足的情况
            //-如果左孩子能借位
            if(left->keys.size() > (M-1)/2){
                //-右孩子向父亲借位
                right->keys.push_front(cur->keys[i]);
                cur->keys.erase(cur->keys.begin()+i);
                //-父亲向左孩子借位
                cur->keys.insert(cur->keys.begin()+i,left ->keys.back());
                left ->keys.pop_back();
                //-左孩子的children随着借位移到右孩子的children上
                right -> children.push_front(left->children.back());
                left->children.pop_back();
                //-在右孩子里继续递归
                deleteTrace(right,key);
            }
            //-如果左孩子也不够,就让cur下沉合并
            else{
                //-cur下沉的key融入右孩子
                right->keys.push_front(cur->keys[i]);
                //-删除cur下沉的key
                cur->keys.erase(cur->keys.begin()+i);
                //-左孩子融入右孩子
                for(int index = 0;index<(M-1)/2;++index){
                    right->keys.push_front(left->keys.back());
                    left -> keys.pop_back();
                    right->children.push_front(left->children.back());
                    left -> children.pop_back();
                }
                //-children的数量会多一位，还要操作一次
                right->children.push_front(left->children.back());
                left -> children.pop_back();

                //-下沉后删除cur多余的children
                cur -> children.erase(cur->children.begin()+i);

                //-销毁左节点
                delete left;

                //-如果cur是root且下沉后keys为空
                if(cur == root && cur->keys.empty()){
                    root = right;
                    delete cur;
                }
                deleteTrace(right, key);
            }
        }
        //-如果孩子的keys数量>（M-1）/2,直接递归即可
        else{
            deleteTrace(right,key);
        }
    }
    //-如果cur找不到删除元素，且key可能在cur->keys[i]的左侧孩子里
    else if(key!=cur -> keys[i]){
        //-获取左右孩子
        Btree_node*left = cur -> children[i];
        Btree_node * right = cur -> children[i+1];
        //-存在孩子节点删除节点会破坏keys的数量下限的可能性，每次都要判断对应孩子的keys是否刚刚好够（M-1）/2
        if(left->keys.size() == (M-1)/2){
            //-如果右孩子能借位
            if(right->keys.size() > (M-1)/2){
                //-左孩子向父亲借位
                left->keys.push_back(cur->keys[i]);
                cur->keys.erase(cur->keys.begin()+i);
                //-父亲向右孩子借位
                cur->keys.insert(cur->keys.begin()+i,right ->keys.front());
                right ->keys.pop_front();
                //-右孩子的children随着借位移到左孩子的children上
                left -> children.push_back(right->children.front());
                right->children.pop_front();
                //-在左孩子里继续递归
                deleteTrace(left,key);
            }
            //-如果右孩子也不够,就让cur下沉合并
            else{
                //-cur下沉的key融入右孩子
                right->keys.push_front(cur->keys[i]);
                //-删除cur下沉的key
                cur->keys.erase(cur->keys.begin()+i);

                //-左孩子融入右孩子
                for(int index = 0;index<(M-1)/2;++index){
                    right->keys.push_front(left->keys.back());
                    left -> keys.pop_back();
                    right->children.push_front(left->children.back());
                    left -> children.pop_back();
                }
                //-children的数量会多一位，还要操作一次
                right->children.push_front(left->children.back());
                left -> children.pop_back();

                //-下沉后删除cur多余的children
                cur -> children.erase(cur->children.begin()+i);

                //-销毁左节点
                delete left;

                //-如果cur是root且下沉后keys为空
                if(cur == root && cur->keys.empty()){
                    root = right;
                    delete cur;
                }
                deleteTrace(right, key);
            }
        }
        //-如果孩子的keys数量>（M-1）/2,直接递归即可
        else{
            deleteTrace(left,key);
        }
    }
    //-如果在cur查找到了keys
    else {
        //-获取左右孩子
        Btree_node*left = cur -> children[i];
        Btree_node * right = cur -> children[i+1];
        //-判断左右孩子是否有可以借位的
        if(left->keys.size()>(M-1)/2){
            //-如果左边的孩子可以借位，就找左孩子的最大值来覆盖删除元素
            cur->keys[i] = left->keys.back();
            left->keys.pop_back();
            return;
        }
        if(right->keys.size()>(M-1)/2){
            //-如果右边的孩子可以借位，就找右孩子的最小值来覆盖删除元素
            cur->keys[i] = right->keys.front();
            right->keys.pop_front();
            return;
        }
        //-如果左右孩子的keys的size刚刚好为(M-1)/2最低下限，则下沉当前节点
        //-cur下沉的key融入右孩子
        right->keys.push_front(cur->keys[i]);
        //-删除cur下沉的key
        cur->keys.erase(cur->keys.begin()+i);
        //-左孩子融入右孩子
        for(int index = 0;index<(M-1)/2;++index){
            right->keys.push_front(left->keys.back());
            left -> keys.pop_back();
            right->children.push_front(left->children.back());
            left -> children.pop_back();
        }
        //-children的数量会多一位，还要操作一次
        right->children.push_front(left->children.back());
        left -> children.pop_back();
        //-下沉后删除cur多余的children
        cur -> children.erase(cur->children.begin()+i);
        //-销毁左节点
        delete left;
        //-如果cur是root且下沉后keys为空
        if(cur == root && cur->keys.empty()){
            root = right;
            delete cur;
        }
        deleteTrace(right,key);
    }
}
//-层序打印B树
void Btree::print(){
    std::deque<Btree_node*> dqueue;
    dqueue.push_back(root);
    while(!dqueue.empty()){
        int size = (int)dqueue.size();
        for (int i = 0; i < size; ++i) {
            Btree_node* temp = dqueue.front();
            dqueue.pop_front();
            std::cout<<"[ ";
            for(auto key:temp->keys){
                std::cout<<key<<" ";
            }
            std::cout<<"] ";
            for(auto child:temp->children){
                if(child!=nullptr){
                    dqueue.push_back(child);
                }
            }
        }
        std::cout<<std::endl;
    }
}

int main(){
    std::unordered_set<KEY_TYPE> kset;
    //-创建一个无重复元素集合
    while(kset.size()<26){
        KEY_TYPE rand_ele = (KEY_TYPE)rand()%1000;
        if(kset.count(rand_ele)>0){
            continue;
        }
        kset.insert(rand_ele);
    }
    std::deque<KEY_TYPE> arr(kset.begin(),kset.end());

    //-六阶B树
    Btree bt(6);
    for(auto key:arr){
        std::cout<<"insert:"<<key<<std::endl;
        bt.insertKey(key);
        //-插入后打印一下B树
        bt.print();
    }

    std::random_shuffle(arr.begin(),arr.end());
    for(auto key:arr){
        std::cout<<"delete:"<<key<<std::endl;
        bt.deleteKey(key);
        bt.print();
    }

    return EXIT_SUCCESS;
}