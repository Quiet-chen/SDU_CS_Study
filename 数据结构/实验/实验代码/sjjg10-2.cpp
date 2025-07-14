#include<iostream>
using namespace std;

template<class T>
struct huffmannode{
    T weight;
    huffmannode<T>* leftchild;
    huffmannode<T>* rightchild;
    bool operator>(const huffmannode b) const{    //运算符重载
        return weight>b.weight;
    }
    bool operator>=(const huffmannode b) const{    
        return weight>=b.weight;
    }
    bool operator<(const huffmannode b) const{    
        return weight<b.weight;
    }
    bool operator<=(const huffmannode b) const{    
        return weight<=b.weight;
    }
    bool operator==(const huffmannode b) const{    
        return weight==b.weight;
    }
    huffmannode(){
        weight=0;
        leftchild=rightchild=NULL;
    }
    huffmannode(const huffmannode<T>& a){
        weight = a.weight;
        leftchild = a.leftchild;
        rightchild = a.rightchild;
    }
    void maketree(huffmannode<T>* a,huffmannode<T>* b){
        weight = a->weight+b->weight;
        leftchild = a;
        rightchild = b;
    }
};



template<class T>
class minheap{
public:
    minheap(){ element=NULL; heapsize=0; length=10;}
    void init(T a[]);
    void push(const T& thelement);
    void pop();
    bool empty(){ return heapsize==0;}
    T peak(){ return element[1];}
    int size(){ return heapsize;}
private:
    T* element;
    int heapsize;
    int length;
};

template <class T>
void minheap<T>::init(T a[]){
    element = new T [27];
    length=27;
    for(int i=1;i<=26;i++){
        if(a[i].weight!=0){
            element[++heapsize]=a[i];
        }
    }
    for(int i=heapsize/2;i>=1;i--){
        T temp = element[i];
        int child = 2*i;
        while(child<=heapsize){
            if(child<heapsize && element[child] > element[child+1]) child++;
            if(temp<element[child]) break;
            element[child/2]=element[child];
            child*=2;
        }
        element[child/2]=temp;
    }
}

template <class T>
void minheap<T>::push(const T& thelement){
    if(heapsize==length-1){
        length*=2;
        T* temp = new T [length];
        for(int i=1;i<=heapsize;i++){
            temp[i]=element[i];
        }
        element=temp;
    }
    int cur = ++heapsize;
    while(cur!=1 && element[cur/2]>thelement){
        element[cur]=element[cur/2];
        cur/=2;
    }
    element[cur]=thelement;
}

template<class T>
void minheap<T>::pop(){
    if(heapsize==0) return ;
    T thelement = element[heapsize--];
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

minheap<huffmannode<int>> tree;
int ans=0;

void preorder(huffmannode<int>* x,int num){
    if(x->leftchild==NULL && x->rightchild==NULL){
        ans+=num * (x->weight);
        return ;
    }
    if(x->leftchild!=NULL) preorder(x->leftchild,++num);
    num--;
    if(x->rightchild!=NULL) preorder(x->rightchild,++num);
}

void huffmantree(){
    huffmannode<int>* x = new huffmannode<int>;
    while(tree.size()>1){
        huffmannode<int> * l =new huffmannode<int> (tree.peak());
        tree.pop();
        huffmannode<int> * r = new huffmannode<int> (tree.peak());
        tree.pop();
        x->maketree(l,r);
        tree.push(*x);
    }
    preorder(x,0);
    cout<<ans;
}

int main(){
    huffmannode<int> node[27];  
    char c;
    c = getchar();
    while(c!= '\n'){
        node[c-'a'+1].weight++;
        c = getchar();
    }
    tree.init(node);
    huffmantree();
}