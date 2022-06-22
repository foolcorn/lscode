#include <iostream>
#include <deque>
#include <string>
#include <vector>

//-既然标题是c++，那么就写成满满的c++风格吧
using Color = bool;//-颜色因为只有红或者黑，选择bool类型
using KEY_TYPE = int;//-为了更好理解红黑树，就不写成模板类了，所以首选万年int（笑~）
using VALUE_TYPE = int;//-同理

//-全局静态红黑变量
static const Color red = false;
static const Color black  = true;

//-红黑树的节点特点，有color，有parent

class RBtree_node{
public:
    Color color;
    RBtree_node * parent;
    RBtree_node * left;
    RBtree_node * right;

    KEY_TYPE key;//-后期如果想解耦合，可以将key和value抽离出去
    VALUE_TYPE value;
    RBtree_node(Color color_):color(color_),parent(nullptr),left(nullptr),right(nullptr),key(-99999){}
    RBtree_node(Color color_, KEY_TYPE key_,RBtree_node * nil):
    color(color_),parent(nil),left(nil),right(nil),key(key_){}
};


class RBtree{
private:
    //-红黑树数据成员：其中nil的意义在于，因为红黑树的所有叶子节点都是黑色的，所以可以将所有临近末尾的节点，
    //-都连接到这一个叶子结点nil上，同理，root的parent也可以连接到nil上，形成一个dummy空节点
    RBtree_node * root;
    RBtree_node * nil;
public :
    //-以下实现了红黑树常用接口：
    //-构造函数
    RBtree(){
    nil = new RBtree_node(black);//-为所有叶子节点nil初始化，颜色为黑色
    root = nil;//-红黑树为空的时候，让nil作为root
}
//-左旋
void leftRotate(RBtree_node *left_node);

//- 右旋
void rightRotate(RBtree_node * right_node);

//-插入key
void insertNode(KEY_TYPE key);

//-修复插入
void fixInsert(RBtree_node * node);

//-查找某个key的节点
RBtree_node* searchNode(KEY_TYPE key);

//-查找某个节点的中序后继
RBtree_node* successor(RBtree_node * node);

//-删除key
void deleteNode(KEY_TYPE key);

//-修复删除
void fixDelete(RBtree_node * node);

//-层序遍历打印红黑树
void print();

//-打印中序遍历
void printMiddle(RBtree_node * node);

};

//-左旋
void RBtree::leftRotate(RBtree_node *left_node){
    RBtree_node * right_node = left_node->right;
    //-右节点的左枝条接在左节点的右枝条上
    left_node->right = right_node->left;
    if(right_node->left!=nil){
        left_node->right->parent = left_node;
    }
    //-右节点接在左节点的父亲上
    right_node->parent = left_node->parent;
    if(left_node == root){
        //-nil不会指向任何节点，但是root->parent是nil
        root = right_node;
    }
    else if(left_node == left_node->parent->left){
        left_node->parent->left = right_node;
    }else{
        left_node->parent->right = right_node;
    }
    //-左节点接在右节点的左枝上
    left_node->parent = right_node;
    right_node->left = left_node;
}

//- 右旋：写完左旋后，把所有left和right对调即可
void RBtree::rightRotate(RBtree_node * right_node){
    RBtree_node * left_node = right_node->left;
    right_node->left = left_node->right;
    if(left_node->right!=nil){
        right_node->left->parent = right_node;
    }
    left_node->parent = right_node->parent;
    if(right_node == root){
        root = left_node;
    }
    else if(right_node == right_node->parent->right){
        right_node->parent->right = left_node;
    }else{
        right_node->parent->left = left_node;
    }
    right_node->parent = left_node;
    left_node->right = right_node;
}

//-插入key
void RBtree::insertNode(KEY_TYPE key){
    RBtree_node * prev = nil;
    RBtree_node * cur = root;
    while(cur!=nil){
        prev = cur;
        if(key>cur->key){
            cur = cur->right;
        }else if(key<cur->key){
            cur = cur->left;
        }else{//-该key已经存在
            return;
        }
    }
    //-创建新节点
    RBtree_node * new_node = new RBtree_node(red,key,nil);
    //-如果节点没有元素
    new_node->parent = prev;
    if(prev == nil){
        root = new_node;
    }
    else if(key<prev -> key){
        prev ->left = new_node;
    }else{
        prev ->right = new_node;
    }
    fixInsert(new_node);
    print();
}

//-修复插入
void RBtree::fixInsert(RBtree_node * new_node){
    while(new_node -> parent->color == red){//-终止条件要注意
        //-如果父亲是左枝
        if(new_node->parent == new_node -> parent->parent->left){
            //-获得其伯父节点
            RBtree_node * uncle = new_node->parent->parent->right;
            if(uncle->color == red){//-如果伯父是红色，那么将父亲和伯父同时变黑，不会破坏左右平衡
                uncle->color = black;
                new_node->parent->color = black;
                new_node->parent ->parent->color = red;//-将祖父变红，才能实现下一轮回溯修复
                new_node = new_node->parent->parent;
            }else{//-如果伯父是黑色
                //-判断new_node是不是右孩子，如果是右孩子转换成左孩子
                if(new_node == new_node -> parent->right){
                    new_node = new_node->parent;
                    leftRotate(new_node);
                }
                //-此时红色节点是左孩子
                //-如果结构本是平衡状态，右边本该比左边多一个黑，但是我们将父亲（左)变黑会破坏平衡，
                //-所以需要右旋祖父，把父亲上浮，相当于在左枝多一个黑的时候给右枝也多了黑，这样左右就能平衡
                new_node->parent->color = black;
                new_node->parent ->parent->color = red;
                rightRotate(new_node->parent->parent);
            }
        }
        //-如果父亲是右枝（将上边代码的left和right全部对调即可，不用记规则）
        else {
            RBtree_node * uncle = new_node->parent->parent->left;
            if(uncle->color == red){//-如果伯父是红色
                uncle->color = black;
                new_node->parent->color = black;
                new_node->parent ->parent->color = red;
                new_node = new_node->parent->parent;
            }else{//-如果伯父是黑色
                if(new_node == new_node -> parent->left){
                    new_node = new_node->parent;
                    rightRotate(new_node);
                }
                new_node->parent->color = black;
                new_node->parent ->parent->color = red;
                leftRotate(new_node->parent->parent);
            }
        }
    }
    //-如果new_node回溯到root，此时root->parent==nil（black）打破了循环，而此时root被改变成了黑色，违反了规则1，
    //-所以最后需要强行把root fix成黑色
    root->color = black;
}

//-查找某个key的节点
RBtree_node* RBtree::searchNode(KEY_TYPE key){
    RBtree_node * cur = root;
    while(cur!=nil){
        if(key>cur -> key){
            cur = cur->right;
        }else if(key < cur -> key){
            cur = cur->left;
        }else{
            return cur;
        }
    }
    return cur;
}

//-查找某个节点的中序后继
RBtree_node* RBtree::successor(RBtree_node * node){
    //-如果节点有右孩子
    if(node->right!=nil){
        RBtree_node * res = node -> right;
        while(res->left!=nil){
            res = res->left;
        }
        return res;
    }else{
        while(node!=root&&node!=node->parent->left){
            node = node->parent;
        }
        return node->parent;
    }
}

//-删除key
void RBtree::deleteNode(KEY_TYPE key){
    //-查找key所在节点
    RBtree_node * key_node = searchNode(key);
    //-实际删除的节点
    RBtree_node* delete_node;
    //-delete_node的孩子
    RBtree_node* delete_son;
    //-如果同时有左枝或者右枝条
    if(key_node->left != nil&&key_node->right != nil){
        delete_node = successor(key_node);
        delete_son = delete_node->right;
    }//-如果仅有左枝或者右枝条或者左右都没有
    else{
        delete_node = key_node;
        if(key_node->left != nil){
            delete_son = key_node->left;
        }else{
            delete_son = key_node->right;
        }
    }

    //-删除deletenode
    delete_son->parent = delete_node->parent;
    //-先判断deletenode是不是根节点
    if(delete_node == root){
        root = delete_son;
    }
    else if(delete_node == delete_node->parent->left){
        delete_node->parent->left = delete_son;
    }else{
        delete_node -> parent -> right = delete_son;
    }
    //-覆盖key_node原有数据
    key_node->key = delete_node -> key;
    key_node ->value = delete_node -> value;

    //-如果删除节点是黑色的，需要修复delete_son,注意是孩子
    if(delete_node->color == black){
        fixDelete(delete_son);
    }
    //-释放空间
    delete delete_node;
    //-打印
    print();
}

//-修复删除
void RBtree::fixDelete(RBtree_node * delete_son){
    //-当删除的黑色节点的孩子是黑色的一直循环
    while(delete_son!=root&&delete_son->color == black){
        //-判断delete_son所在枝条,如果是左枝
        if(delete_son == delete_son->parent->left){
            //-如果兄弟是红色的，此时孩子（左）黑，兄弟右（红）不平衡
            RBtree_node * bro = delete_son->parent->right;
            if(bro->color == red){
                bro->color = black;//-兄弟变黑
                delete_son->parent->color = red;//-父亲变红
                leftRotate(delete_son->parent);//-左旋父亲
                bro = delete_son->parent->right;//-新的bro是原来bro的左枝，因为原bro是红的，其左右枝都是黑色的，这样保证新的兄弟是黑色的
            }
            //-此时兄弟是黑色的,判断兄弟的孩子
            //-左黑右黑（兄弟的孩子平衡了）
            if(bro->left->color == black&&bro->right -> color == black){
                bro->color = red;
                delete_son = delete_son->parent;
            }else{
                //-如果是左红右黑,变成左黑右红
                if (bro->right->color == black){
                    bro -> color = red;
                    bro->left->color = black;
                    rightRotate(bro);
                }
                bro->color = bro->parent -> color;
                bro->parent -> color = black;
                bro->right->color = black;
                leftRotate(delete_son->parent);
                delete_son = root;
            }
        }
        //-如果是右枝（不用记规则，把上面的代码left和right对调即可）
        else {
            RBtree_node * bro = delete_son->parent->left;
            if(bro->color == red){
                bro->color = black;
                delete_son->parent->color = red;
                rightRotate(delete_son->parent);
                bro = delete_son->parent->left;
            }
            if(bro->right->color == black&&bro->left -> color == black){
                bro->color = red;
                delete_son = delete_son->parent;
            }else{
                if (bro->left->color == black){
                    bro -> color = red;
                    bro->right->color = black;
                    leftRotate(bro);
                }
                bro->color = bro->parent -> color;
                bro->parent -> color = black;
                bro->left->color = black;
                rightRotate(delete_son->parent);
                delete_son = root;
            }
        }
    }
    delete_son->color = black;
}

//-层序遍历打印红黑树
void RBtree::print(){
    std::deque<RBtree_node*> dqueue;//-使用deque实现队列
    dqueue.push_back(root);
    while(!dqueue.empty()){
        int size = (int)dqueue.size();
        for (int i = 0; i < size; ++i) {
            RBtree_node* temp = dqueue.front();
            dqueue.pop_front();
            if(temp->left!=nullptr){
                dqueue.push_back(temp -> left);
            }
            if(temp -> right != nullptr){
                dqueue.push_back(temp -> right);
            }
            std::string color = temp->color?"b: ":"r: ";
            std::string keystr = temp==nil?"nil":std::to_string(temp->key);
            std::cout<<color<<keystr<<"  ";
        }
        std::cout<<std::endl;
    }
}

//-打印中序遍历
void RBtree::printMiddle(RBtree_node * node){
    if(node == nil){
        return;
    }
    printMiddle(node->left);
    std::string color = node->color?"b:":"r:";
    std::cout<<color<<std::to_string(node->key)<<"  ";
    printMiddle(node->right);
}

int main(){
    RBtree rb;
    std::string select;
    KEY_TYPE key;
    while(true){
        std::cout<<"\n输入操作：i：插入key，d：删除key q：退出"<<std::endl;
        std::cin>>select;
        if(select == "i"){
            std::cout<<"输入key"<<std::endl;
            std::cin>>key;
            rb.insertNode(key);
        }else if(select == "d"){
            std::cout<<"输入key"<<std::endl;
            std::cin>>key;
            rb.deleteNode(key);
        }else if(select == "q"){
            break;
        }else{
            std::cout<<"输入不合法，重新输入"<<std::endl;
        }
    }
    return 0;
}