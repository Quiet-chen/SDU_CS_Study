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

template <class T>
class binarytree{
public:
    binarytree(){ root = NULL; treesize=0;}
    void visit(binarytreenode<T>* x){
        cout<<x->element<<' ';
    }
    int tsize(){ return treesize;}
    bool empty(){ return treesize==0;}
    void postorder(binarytreenode<T>* &t);
    binarytreenode<T>* root;
private:
    int treesize;
};

template<class T>
void binarytree<T>::postorder(binarytreenode<T>* &t){
    if(t!=NULL){
        postorder(t->leftchild);
        postorder(t->rightchild);
        visit(t);
    }
}


int n;
int pre[100000]={0};
int in[100000]={0};
int root_i=1;

template<class T>
void createtree(int left,int right,binarytreenode<T>* &theroot){
    if(left>right) return ;
    for(int i=left;i<=right;i++){
        if(pre[root_i]==in[i]){
            if(theroot==NULL){
                theroot = new binarytreenode<T> (pre[root_i]);
                root_i++;
                createtree(left,i-1,theroot->leftchild);
                createtree(i+1,right,theroot->rightchild);
            }
        }
    }
}


int main(){
    cin>>n;
    for(int i=1;i<=n;i++) cin>>pre[i];
    for(int i=1;i<=n;i++) cin>>in[i];
    binarytree<int> tree;
    createtree(1,n,tree.root);
    tree.postorder(tree.root);
}