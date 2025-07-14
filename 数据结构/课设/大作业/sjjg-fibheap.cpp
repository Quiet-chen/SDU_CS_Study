#include<iostream>
#include<cmath>
#include<vector>
using namespace std;

template<class T>
struct fibnode{
    fibnode<T>* left;
    fibnode<T>* right;
    fibnode<T>* child;
    fibnode<T>* parent;
    int degree;
    bool mark;
    T key;
    fibnode(T k){
        key=k;
        degree=0;
        mark=false;
        child=NULL;
        parent=NULL;
    }
};

template<class T>
class fibheap{
public:
    fibheap(){
        n=0;
        themin=NULL;
    }
    fibheap<T> make_heap(){
        fibheap<T> fib_heap;
        return fib_heap;
    }
    void insert(fibnode<T>* x);
    fibnode<T>* minimun(){
        return themin;
    }
    fibheap<T>* union_fibheap(fibheap<T>& H1,fibheap<T>& H2);
    void extract_min();
    void consolidate();
    void nodelink(fibnode<T>* y, fibnode<T>* x);
    void decrease(fibnode<T>* x, T k);
    void cut(fibnode<T>* x, fibnode<T>* y);
    void cascading_cut(fibnode<T>* y);
    void deletenode(fibnode<T>* x){
        int tt=themin->key-10;
        decrease(x,tt);
        extract_min();
    }
    void show(){                     //按结构输出所有结点
        if(themin==NULL) return;
        fibnode<T>* z=themin->right;
        cout<<themin->key;
        show_node(themin->child,2);
        while(z!=themin){
            cout<<z->key;
            show_node(z->child,2);
            z=z->right;
        }
    }
    void show_node(fibnode<T>* thenode, int n){
        if(thenode==NULL){
            cout<<'\n';
            return ;
        }
        else{
            fibnode<T>* z=thenode->right;
            for(int i=0;i<n*2-1;i++) cout<<' ';
            cout<<thenode->key;
            show_node(thenode->child,++n);
            n--;
            while(z!=thenode){
                for(int i=0;i<n*2;i++) cout<<' ';
                cout<<z->key;
                show_node(z->child,++n);
                n--;
                z=z->right;
            }
        }
    }
private:
    int n;
    fibnode<T>* themin;
};

template<class T>
void fibheap<T>::insert(fibnode<T>* x){ //直接在min结点旁插入
    if(themin==NULL){
        x->left=x;
        x->right=x;
        themin=x;
    }
    else{
        x->left=themin->left;
        themin->left->right=x;
        themin->left=x;
        x->right=themin;
        if(x->key<themin->key) themin=x;
    }
    n++;
}

template<class T>
fibheap<T>* fibheap<T>::union_fibheap(fibheap<T>& H1,fibheap<T>& H2){ //合并两个堆
    themin=H1.themin;
    H2.themin->left->right=themin->right;
    themin->right->left=H2.themin->left;
    themin->right=H2.themin;
    H2.themin->left=themin;
    if(H1.themin==NULL && H2.themin!=NULL){
        themin=H2.themin;
    }
    else if(H2.themin!=NULL && H1.themin!=NULL&&H1.themin->key>H2.themin->key){
        themin=H2.themin;
    }
    n=H1.n+H2.n;
    return this;
}

template<class T>
void fibheap<T>::extract_min(){      //删除最小结点
    fibnode<T>* z=themin;
    if(z!=NULL){
        fibnode<T>* w=z->child;
        for(int i=0;i<z->degree;i++){  //把min结点的所有孩子插入根链表
            fibnode<T>* x=w;
            w=w->right;
            x->left->right=x->right;
            x->right->left=x->left;
            x->parent=NULL;
            x->mark=false;
            insert(x);
        }
        z->child=NULL;
        if(z==z->right) themin=NULL;  //从根链表中把min结点删除
        else{
            z->left->right=z->right;
            z->right->left=z->left;
            themin=z->right;
            consolidate();
        }
        n--;
    }
}

template<class T>
void fibheap<T>::consolidate(){      //合并根链表
    int d=(int)floor(log(n)/log(2));
    fibnode<T>* A[d+1];
    for(int i=0;i<=d;i++) A[i]=NULL;
    fibnode<T>* w=themin;
    int rootn=1;
    while(w->right!=themin){
        rootn++;
        w=w->right;
    }
    w=themin;
    for(int i=0;i<rootn;i++){  //找所有度数相同的结点
        fibnode<int>* x=w;
        int de=x->degree;
        while(A[de]!=NULL){
            fibnode<T>* y=A[de];
            if(x->key>y->key){
                w=y;
                fibnode<T>* temp=x;
                x=y;
                y=temp;
            }
            nodelink(y,x);     //将y链接到x
            A[de]=NULL;
            de++;
        }
        A[de]=x;
        w=w->right;
    }
    themin=NULL;
    for(int i=0;i<=d;i++){
        if(A[i]!=NULL) insert(A[i]);//将度数数组的节点连成根链表
    }
}

template<class T>
void fibheap<T>::nodelink(fibnode<T>* y, fibnode<T>* x){
    y->parent=x;
    y->left->right=y->right;
    y->right->left=y->left;
    x->degree++;
    if(x->child==NULL){
        x->child=y;
        y->left=y;
        y->right=y;
    }
    else{
        y->right=x->child->right;
        x->child->right->left=y;
        x->child->right=y;
        y->left=x->child;
    }
    y->mark=false;
}

template<class T>
void fibheap<T>::decrease(fibnode<T>* x, T k){//x减值到k
    if(k>x->key) return ;
    x->key=k;
    fibnode<T>* y=x->parent;
    if(y!=NULL && x->key<y->key){
        cut(x,y);
        cascading_cut(y);
    }
    if(x->key<themin->key) themin=x;
}

template<class T>
void fibheap<T>::cut(fibnode<T>* x, fibnode<T>* y){//x从y处切断
    if(y->child==x){
        if(y->degree<=1) y->child=NULL;
        else y->child=x->right;
    }
    x->left->right=x->right;
    x->right->left=x->left;
    n--;
    y->degree--;
    n--;
    insert(x);           //插入到根链表
    x->parent=NULL;
    x->mark=false;
}

template<class T>
void fibheap<T>::cascading_cut(fibnode<T>* y){//如果y是true，将y也切断
    fibnode<T>* z=y->parent;
    if(z!=NULL){
        if(y->mark==false){
            y->mark=true;
        }
        else{
            cut(y,z);
            cascading_cut(z);
        }
    }
}

int main(){
    fibheap<int> fib_heap;
    int n;
    cout<<"input n:"<<endl;
    cin>>n;
    cout<<"input number of n:"<<endl;
    while(n){
        int t;
        cin>>t;
        fibnode<int>* temp=new fibnode<int> (t);
        fib_heap.insert(temp);
        fib_heap.consolidate();
        n--;
    }
    cout<<"after insert and consolidate:"<<endl;
    
    fib_heap.show();
    fibnode<int>* temp=new fibnode<int> (45);
    fib_heap.insert(temp);
    cout<<"insert 45:"<<endl;
    fib_heap.consolidate();
    fib_heap.show();
    cout<<"after pop min:"<<endl;
    fib_heap.extract_min();
    fib_heap.show();
    cout<<"after delete 45:\n";
    fib_heap.deletenode(temp);
    fib_heap.show();
    cout<<"input another fibheap n:"<<endl;
    int m;
    cin>>m;
    fibheap<int> fib_heap2;
    cout<<"input another n number:"<<endl;
    while(m){
        int t;
        cin>>t;
        fibnode<int>* tp=new fibnode<int> (t);
        fib_heap2.insert(tp);
        m--;
    }
    fib_heap2.consolidate();
    cout<<"after consolidate:"<<endl;
    fib_heap2.show();
    fibheap<int> fib_heap3;
    cout<<"two heap union:"<<endl;
    fib_heap3.union_fibheap(fib_heap,fib_heap2);
    fib_heap3.show();
}