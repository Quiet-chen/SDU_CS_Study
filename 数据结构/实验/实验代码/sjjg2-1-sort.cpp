#include <iostream>
using namespace std;

template<class T>
void myswap(T& a,T& b){ //交换函数
    T temp=a;
    a=b;
    b=temp;
}

template<typename T>
class mySort{
public:
    mySort(int n,T a[]){  //构造函数把数组和n赋给指针p和num
        p=new T [n];
        for(int i=0;i<n;i++) p[i]=a[i];
        num=n;
    }
    void sort_rank();
    void sort_select();
    void sort_bubble();
    void sort_insert();
    void show();
    ~mySort(){delete [] p;}
private:
    T* p;  //待排序数组
    int num; //数组元素个数
};

template<typename T>
void mySort<T>::sort_rank(){   //名次排序
    int r[1005]={0};
    for(int i=0;i<num;i++){
        for(int j=i+1;j<num;j++){
            if(p[i]<=p[j]) r[j]++; //计算名词，哪个大哪个名词+1
            else r[i]++;
        }
    }
    for(int i=0;i<num;i++){
        while(r[i]!=i){
            int t=r[i];
            myswap(p[i],p[t]);  //原地重排，不断循环直到该位置是该名词的数
            myswap(r[i],r[t]);
        }
    }
}

template<typename T>
void mySort<T>::sort_select(){  //选择排序
    bool check=false;
    int last=num;
    while(!check&&last>1){
        int temp=0;
        check=true;
        for(int i=1;i<last;i++){
            if(p[temp]<=p[i]) temp=i; //寻找最大值，若每次循环都改变最大值下标说明数组从小到大有序
            else check=false;          //数组无序
        }
        myswap(p[temp],p[--last]);//把最大值放最后
    }
}

template<typename T>
void mySort<T>::sort_bubble(){ //冒泡排序
    bool check=false;
    int last=num;
    while(!check&&last>1){
        check=true;
        for(int i=0;i<last-1;i++){
            if(p[i]>p[i+1]){
                myswap(p[i],p[i+1]);//有交换标记为false，无交换说明数组有序
                check=false;
            }
        }
        last--;
    }
}

template<typename T>
void mySort<T>::sort_insert(){ //插入排序
    for(int i=1;i<num;i++){
        int temp=p[i];
        int j=i-1;
        for( ;j>=0;j--){
            if(temp<p[j]){ //待插入数小，被比较数往后移
                p[j+1]=p[j];
            }
            else{
                break;
            }
        }
        p[j+1]=temp; //将待插入数放入空位
    }
}

template<typename T>
void mySort<T>::show(){  //输出有序数组
    for(int i=0;i<num;i++){ 
        cout<<p[i]<<" ";
    }
    cout<<endl;
}

int main(){
    int r[1005]={0};
    int a[1005]={0};
    int n;
    cin>>n;
    for(int i=0;i<n;i++) cin>>a[i];
    mySort<int> A(n,a); 
    A.sort_select();
    A.show();
}