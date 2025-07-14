#include<iostream>
using namespace std;

template<class T>
class minheap{
public:
    minheap(){ element=NULL; heapsize=0; length=10;}
    void init(int n);
    void push(const T& thelement);
    void pop();
    bool empty(){ return heapsize==0;}
    void peak(){ cout<<element[1];}
private:
    T* element;
    int heapsize;
    int length;
};

template <class T>
void minheap<T>::init(int n){
    heapsize=n;
    length=2*n;
    element = new T [length];
    for(int i=1;i<=heapsize;i++){
        cin>>element[i];
    }
    for(int i=heapsize/2;i>=1;i--){             //检查每个根节点与子树的关系
        T temp = element[i];
        int child = 2*i;
        while(child<=heapsize){
            if(child<heapsize && element[child] > element[child+1]) child++;
            if(temp<element[child]) break;                                    //找到最小的元素
            element[child/2]=element[child];
            child*=2;
        }
        element[child/2]=temp;
    }
}

template <class T>
void minheap<T>::push(const T& thelement){
    if(heapsize==length-1){                  //空间扩大
        length*=2;
        T* temp = new T [length];
        for(int i=1;i<=heapsize;i++){
            temp[i]=element[i];
        }
        element=temp;
    }
    int cur = ++heapsize;
    while(cur!=1 && element[cur/2]>thelement){//根节点比插入的元素大，根节点下放
        element[cur]=element[cur/2];
        cur/=2;
    }
    element[cur]=thelement;
    cout<<element[1]<<endl;
}

template<class T>
void minheap<T>::pop(){
    if(heapsize==0) return ;
    T thelement = element[heapsize--];    //删除根节点，把最后一个叶节点往上放
    int cur = 1;
    int child = 2;
    while(child<=heapsize){
        if(child<heapsize && element[child]>element[child+1]) child+=1;
        if(thelement<=element[child]) break;
        element[cur]=element[child];
        cur=child;
        child*=2;
    }
    element[cur]=thelement;
}

template<class T>
void heapsort(minheap<T>& h){
    while(!h.empty()){
        h.peak();
        h.pop();
        cout<<' ';
    }
}

int main(){
    int n;
    cin>>n;
    minheap<int> H;
    H.init(n);
    H.peak();
    cout<<endl;
    int m;
    cin>>m;
    for(int i=0;i<m;i++){
        int p;
        cin>>p;
        if(p==1){
            int num;
            cin>>num;
            H.push(num);
        }
        if(p==2){
            H.pop();
            H.peak();
            cout<<endl;
        }
        if(p==3){
            int t;
            cin>>t;
            minheap<int> H2;
            H2.init(t);
            heapsort(H2);
        }
    }
}