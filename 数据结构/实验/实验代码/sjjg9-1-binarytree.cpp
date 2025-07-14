#include <iostream>
using namespace std;

template <class T>
struct binarytreenode{
    T element;
    binarytreenode<T>* leftchild,
                     * rightchild;
    binarytreenode(){
        element=0;
        leftchild=rightchild=NULL;
    }
    binarytreenode(const T& thelement){
        element=thelement;
        leftchild=NULL;
        rightchild=NULL;
    }
    binarytreenode(const T& thelement, binarytreenode<T>* theleftchild, binarytreenode<T>* therightchild){
        element=thelement;
        leftchild=theleftchild;
        rightchild=therightchild;
    }
};

template<class T>
class mystack{   //数组描述
public:
    mystack(int n=10);
    bool empty();
    int size();
    T top();
    void pop();
    void push(T& thelement);
    ~mystack();
private:
    T* element;
    int stacktop;
    int stacksize;
};

template<class T>
mystack<T>::mystack(int n){
    stacksize=n;
    element=new T [n];
    stacktop=-1;
}

template<class T>
bool mystack<T>::empty(){
    return stacktop==-1;
}

template <class T>
int mystack<T>::size(){
    return stacktop+1;
}

template <class T>
T mystack<T>::top(){
    if(stacktop!=-1){
        return element[stacktop];
    }
    else{
        return 0;
    }
}

template <class T>
void mystack<T>::pop(){
    if(stacktop!=-1){
        element[stacktop--]=0;
    }
}

template <class T>
void mystack<T>::push(T& thelement){
    if(stacktop+1==stacksize){                    //若空间不够，重新进行动态分配
        stacksize*=2;
        T* temp = new T [stacksize];
        for(int i=0;i<stacksize;i++){
            temp[i]=element[i];
        }
        element=temp;
    }
    element[++stacktop]=thelement;
}

template<class T>
mystack<T>::~mystack(){
    stacksize=0;
    stacktop=-1;
    T* p=element;
    delete [] p;
    element=NULL;
}

template<class T>
class myqueue{
public:
    myqueue(int n=100);
    bool empty();
    void push(T& thelement);
    void pop();
    T front();
    T back();
    int size();
private:
    int queuefront;
    int queueback;
    int queuelength;
    int queuesize;
    T* element;
};

template<class T>
myqueue<T>::myqueue(int n){
    element=new T [n];
    queuefront=n-1;
    queueback=n-1;
    queuelength=n;
    queuesize=0;
}

template <class T>
bool myqueue<T>::empty(){
    return queuefront == queueback;
}

template <class T>
void myqueue<T>::push(T& thelement){
    if(queuefront==(queueback+1)%queuelength){                    //若空间不够，重新进行动态分配
        queuelength*=2;
        T* temp = new T [queuelength];
        for(int i=0;i<queuelength;i++){
            temp[i]=element[i];
        }
        element=temp;
    }
    queueback = (queueback+1)%queuelength;
    element[queueback]=thelement;
    queuesize++;
}

template<class T>
void myqueue<T>::pop(){
    queuefront=(queuefront+1)%queuelength;
    element[queuefront]=0;
    queuesize--;
}

template<class T>
T myqueue<T>::front(){
    return element[(queuefront+1)%queuelength];
}

template<class T>
T myqueue<T>::back(){
    return element[queueback];
}

template<class T>
int myqueue<T>::size(){
    return queuesize;
}

template <class T>
class binarytree{
public:
    binarytree(){ root = NULL; treesize=0;}
    void init();
    void visit(binarytreenode<T>* x){
        cout<<x->element<<' ';
    }
    bool empty(){ return treesize==0;}
    void preorder(binarytreenode<T>* t);
    void inorder(binarytreenode<T>* t);
    void postorder(binarytreenode<T>* t);
    void levelorder(binarytreenode<T>* t);
    void size();
    int size(binarytreenode<T>* t);
    void height();
    int height(binarytreenode<T>* t);
    binarytreenode<T>* root;
private:
    int treesize;
};

template<class T>
void binarytree<T>::init(){
    int n;
    cin>>n;
    treesize = n;
    myqueue<binarytreenode<T>*> Q;
    root = new binarytreenode<T> (1);
    binarytreenode<T>* t = root;
    T l[n+1]={0};
    T r[n+1]={0};
    for(int i=1;i<=n;i++) cin>>l[i]>>r[i];
    for(int i=1;i<=n;i++){
        int num = t->element;
        if(l[num]!=-1){
            t->leftchild = new binarytreenode<T> (l[num]);
            Q.push(t->leftchild);
        }
        if(r[num]!=-1){
            t->rightchild = new binarytreenode<T> (r[num]);
            Q.push(t->rightchild);
        }
        if(!Q.empty()){
            t=Q.front();
            Q.pop();
        }
    }
} 

template<class T>
void binarytree<T>::preorder(binarytreenode<T>* t){
    if(t!=NULL){
        visit(t);
        preorder(t->leftchild);
        preorder(t->rightchild);
    }
}

template<class T>
void binarytree<T>::inorder(binarytreenode<T>* t){
    mystack<binarytreenode<T>*> Q;
    while(t!=NULL || !Q.empty()){
        if(t!=NULL){
            Q.push(t);
            t=t->leftchild;
        }
        else{
            if(Q.empty()) return ;
            t=Q.top();
            visit(t);
            Q.pop();
            t=t->rightchild;
        }
    }
}

template<class T>
void binarytree<T>::postorder(binarytreenode<T>* t){
    mystack<binarytreenode<T>*> Q;
    binarytreenode<T>* pre=NULL;
    while(t!=NULL || !Q.empty()){
        if(t!=NULL){
            Q.push(t);
            t=t->leftchild;
        }
        else{
             t=Q.top();
            if(t->rightchild==NULL || t->rightchild==pre){//当前节点是一个根节点
                visit(t);
                Q.pop();
                pre=t;//修改访问过的节点
                t=NULL;
            }
            else//右子树还未被遍历
                t=t->rightchild;
        }
    }
}

template<class T>
void binarytree<T>::levelorder(binarytreenode<T>* t){
    myqueue<binarytreenode<T>*> Q;
    while(t!=NULL){
        visit(t);
        if(t->leftchild!=NULL) Q.push(t->leftchild);
        if(t->rightchild!=NULL) Q.push(t->rightchild);
        if(!Q.empty()){
            t=Q.front();
            Q.pop();
        }
        else{
            t=NULL;
        }
    }
}

template <class T>
void binarytree<T>::size(){
    binarytreenode<T>* t = root;
    myqueue<binarytreenode<T>*> Q;
    int s[100001]={0};
    while(t!=NULL){
        s[t->element] = size(t);
        if(t->leftchild!=NULL) Q.push(t->leftchild);
        if(t->rightchild!=NULL) Q.push(t->rightchild);
        if(!Q.empty()){
            t=Q.front();
            Q.pop();
        }
        else{
            t=NULL;
        }
    }
    for(int i=1;i<=treesize;i++) cout<<s[i]<<' ';
}

template<class T>
int binarytree<T>::size(binarytreenode<T>* t){
    if(t!=NULL){
        return 1+size(t->leftchild)+size(t->rightchild); 
    }
    else{
        return 0;
    }
}

template <class T>
void binarytree<T>::height(){
    binarytreenode<T>* t = root;
    myqueue<binarytreenode<T>*> Q;
    int h[100001]={0};
    while(t!=NULL){
        h[t->element] = height(t);
        if(t->leftchild!=NULL) Q.push(t->leftchild);
        if(t->rightchild!=NULL) Q.push(t->rightchild);
        if(!Q.empty()){
            t=Q.front();
            Q.pop();
        }
        else{
            t=NULL;
        }
    }
    for(int i=1;i<=treesize;i++) cout<<h[i]<<' ';
}

template<class T>
int binarytree<T>::height(binarytreenode<T>* t){
    if(t==NULL){
        return 0;
    }
    int hl=height(t->leftchild);
    int hr=height(t->rightchild);
    if(hl>hr) return ++hl;
    else return ++hr;
}

int main(){
    binarytree<int> tree;
    tree.init();
    tree.preorder(tree.root);
    cout<<endl;
    tree.inorder(tree.root);
    cout<<endl;
    tree.postorder(tree.root);
    cout<<endl;
    tree.levelorder(tree.root);
    cout<<endl;
    tree.size();
    cout<<endl;
    tree.height();
}