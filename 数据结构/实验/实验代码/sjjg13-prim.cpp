#include <iostream>
using namespace std;

struct edge{
    int u,v,weight;
    bool operator>(const edge b) const{    //运算符重载
        return weight>b.weight;
    }
    bool operator>=(const edge b) const{    
        return weight>=b.weight;
    }
    bool operator<(const edge b) const{    
        return weight<b.weight;
    }
    bool operator<=(const edge b) const{    
        return weight<=b.weight;
    }
    bool operator==(const edge b) const{    
        return weight==b.weight;
    }
};

template <class T>
class myarray{
public:
    myarray(int capacity=10);
    void insert(T& newelement);       
    int arraysize(){
        return size;
    }
    ~myarray(){
        delete [] element;
        size=0;
        length=0;
    }
    T* element;
private:
    int length;
    int size;
};

template <class T>
myarray<T>::myarray(int capacity){   //构造函数
    element = new T[capacity];
    length=capacity;
    size=0;
}

template <class T>
void myarray<T>::insert(T& newelement){   //在数组最后插入新元素
    if(size>=length){                    //若空间不够，重新进行动态分配
        length+=5000;
        T* temp = new T [length];
        for(int i=0;i<size;i++){
            temp[i]=element[i];
        }
        delete [] element;
        element=temp;
    }
    element[size++]=newelement;     //队尾插入
}

template<class T>
class minheap{
public:
    minheap(){ element=NULL; heapsize=0; length=0;}
    void push(const T thelement);
    void pop();
    bool empty(){ return heapsize==0;}
    T top(){ return element[1];}
private:
    T* element;
    int heapsize;
    int length;
};

template <class T>
void minheap<T>::push(const T thelement){
    if(heapsize>=length-1){                  //空间扩大
        length+=5000;
        T* temp = new T [length];
        for(int i=1;i<=heapsize;i++){
            temp[i]=element[i];
        }
        delete [] element;
        element=temp;
    }
    int cur = ++heapsize;
    while(cur!=1 && element[cur/2]>thelement){//根节点比插入的元素大，根节点下放
        element[cur]=element[cur/2];
        cur/=2;
    }
    element[cur]=thelement;
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

int check[500005]={0};

void prim(myarray<edge>* edg, int n, int e){
    long long ans=0;
    int edge_tree=0;
    minheap<edge> H;
    check[1]=1;
    for(int i=0;i<edg[1].arraysize();i++){          //把点1相连的边放入最小堆
        H.push(edg[1].element[i]);
    }
    while(edge_tree<n-1){
        int minn;
        int to;
        while(check[H.top().v]==1) H.pop();     //如果另一个点是被访问过的，就把边pop掉
        minn = H.top().weight;
        to = H.top().v;                         //找到一个新的点且边权是剩下的边里最小的
        H.pop();
        ans+=minn;
        check[to]=1;
        for(int i=0;i<edg[to].arraysize();i++){          //把新的点相邻的边如果另一个点未被访问过则放入最小堆
            if(check[edg[to].element[i].v]==0) H.push(edg[to].element[i]);
        }
        edge_tree++;
    }
    printf("%ld",ans);
}

myarray<edge>* edg = new myarray<edge> [500005];

int main(){
    int n,e;
    scanf("%d%d",&n,&e);
    for(int i=1;i<=e;i++){
        int u,v,w;
        scanf("%d%d%d",&u,&v,&w);
        edge e1,e2;
        e1.weight=w;
        e2.weight=w;
        e1.v=v,e2.v=u;
        e1.u=u,e2.u=v;
        edg[u].insert(e1);               //无向图，一条边看作两条边
        edg[v].insert(e2);
    }
    prim(edg,n,e);
}