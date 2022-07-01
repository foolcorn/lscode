/*
 * 实现一些冷门排序：
 * 比较顺序：希尔排序，堆排序
 * 非比较算法：计数排序，桶排序，基数排序
 */
#include <iostream>
#include <vector>
using namespace std;

//-希尔排序
class ShellSort{
    public:
    // *希尔排序：平均时间复杂度：O(nlogn)，空间复杂度O(1)
    //- 希尔排序的思路是：每次都取一个增量，该增量为数组长度的逐层折半
    void shellSort(vector<int> &nums){
        for(int incre = nums.size()/2; incre >0; incre/=2){
            //-从第一个增量下标开始遍历,并且把该下标按增量插入到之前的合适位置
            for(int index = incre; index < nums.size(); ++index){
                int last_index = index;
                for(int pre_index = index-incre;pre_index>=0;pre_index-=incre){
                    if(nums[last_index]<nums[pre_index]){
                        swap(nums[last_index],nums[pre_index]);
                        last_index = pre_index;
                    }else{
                        break;
                    }
                }
            }
        }
    }
};

//-堆排序
class HeapSort{
private:
    //-堆是完全二叉树,使用数组来实现堆
    vector<int> treelist = {0};
public:
    void heapSort(vector<int> &nums){
        if(nums.size() == 0){
            cout<<"数组无数据"<<endl;
            return;
        }
        createHeap(nums);
        popHeap(nums);
    }

    void createHeap(vector<int> &nums){
        treelist.push_back(nums[0]);
        for(int i = 1; i < nums.size(); i++){
            //-先将新元素插到树底,然后逐层向上修复
            treelist.push_back(nums[i]);
            //-修复小顶堆
            fixTreeBottom();
        }
    }

    void popHeap(vector<int> &nums){
        nums.clear();
        //-不断从小顶堆pop堆顶元素到nums,并进行修复
        while(treelist.size()>1){
            nums.push_back(treelist[1]);
            //-将最后一个节点移到根节点
            swap(treelist[1],treelist.back());
            treelist.pop_back();
            //-自顶向下修复堆
            fixTreeTop();
        }
    }

    //-自底向上修复
    void fixTreeBottom(){
        //-cur=新节点的index
        int cur = (int)treelist.size()-1;
        int parent = cur/2;
        while(cur!=1){
            //-如果cur比parent小，要修复
            if(treelist[cur]<treelist[parent]){
                swap(treelist[cur],treelist[parent]);
            }else{
                break;
            }
            cur = parent;
            parent = cur/2;
        }
    }

    //-自顶向下修复堆
    void fixTreeTop(){
        int cur = 1;
        int left_son = cur*2;
        int right_son = cur*2+1;
        while(left_son<treelist.size()||right_son < treelist.size()){
            //-如果只有左孩子
            int min_son = left_son;
            //-如果左右孩子都在
            if(right_son<treelist.size()){
                min_son = treelist[left_son]<treelist[right_son]?left_son:right_son;
            }
            if(treelist[cur]>treelist[min_son]){
                swap(treelist[cur],treelist[min_son]);
            }else{
                break;
            }
            cur = min_son;
            left_son = cur*2;
            right_son = cur*2+1;
        }
    }

};

//-计数排序
class CountSort{
    public:
    void countSort(vector<int> &nums) {
//        vector<int> nums
        //-求最大值和最小值
        int maxele = INT_MIN;
        int minele = INT_MAX;
        for(auto &ele:nums){
            maxele = max(maxele,ele);
            minele = min(minele,ele);
        }
        //-根据极值范围创建数组
        vector<int> temp(maxele-minele+1,0);
        for(auto &ele:nums){
            ++temp[ele-minele];
        }
        nums.clear();
        for(int i = 0;i<temp.size();++i){
            nums.insert(nums.end(),temp[i],i+minele);
        }
    }
};

//-桶排序
class BucketSort{
    void bucketSort(vector<int> &nums){

    }

};

//-基数排序
class RadixSort{

    void radixSort(vector<int> &nums){

    }

};

//-打印vec
void printVec(const vector<int> & nums){
    for(auto num:nums){
        cout<<num << " ";
    }
    cout << endl;
}

//-小测试
int main(){
    vector<int> nums;
    for(int i = 0;i<12;++i){
        nums.push_back(rand()%1000);
    }
    printVec(nums);
    //-希尔排序
//    ShellSort ss;
//    ss.shellSort(nums);
    //-堆排序
//    HeapSort hs;
//    hs.heapSort(nums);
    //-计数排序
//    CountSort cs;
//    cs.countSort(nums);

    printVec(nums);

    exit(EXIT_SUCCESS);
}