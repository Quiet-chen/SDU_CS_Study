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

template<class T>
class minheap{
public:
    minheap(){ element=NULL; heapsize=0; length=10;}
    void init(int n);
    void push(const T& thelement);
    void pop();
    bool empty(){ return heapsize==0;}
    T top(){ return element[1];}
private:
    T* element;
    int heapsize;
    int length;
};

template <class T>
void minheap<T>::init(int n){
    heapsize=n;
    length=2*n;
    element = new T [500005];
    for(int i=1;i<=heapsize;i++){
        cin>>element[i].u>>element[i].v>>element[i].weight;
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

int father[500005]={0};

int find(int i){
    if(i==father[i]) return i;
    int t=find(father[i]);
    father[i]=t;
    return t;
} 

template<class T>
void kruskal(minheap<T> H, int n){
    edge temp;
    unsigned long long ans=0;
    int edge_tree=0;
    while(!H.empty() && edge_tree<n-1){
        temp = H.top();
        H.pop();
        int t1=find(temp.u);
        int t2=find(temp.v);
        if(t1!=t2){
            father[t1]=t2;
            ans+=temp.weight;
            edge_tree++;
        }
    }
    cout<<ans;
}

int main(){
    int n,e;
    cin>>n>>e;
    minheap<edge> H;
    H.init(e);
    for(int i=1;i<=n;i++){
        father[i]=i;
    }
    kruskal(H,n);
}