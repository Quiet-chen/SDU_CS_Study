#include <iostream>
#include <utility>
using namespace std;

template <class K,class E>
struct binarytreenode{
    pair<K,E> element;
    binarytreenode<K,E>* leftchild,
                       * rightchild;
    binarytreenode(){
        element=make_pair(0,0);
        leftchild=rightchild=NULL;
    }
    binarytreenode(const pair<K,E> thelement){
        element=thelement;
        leftchild=NULL;
        rightchild=NULL;
    }
    binarytreenode(const pair<K,E> thelement, binarytreenode<K,E>* theleftchild, binarytreenode<K,E>* therightchild){
        element=thelement;
        leftchild=theleftchild;
        rightchild=therightchild;
    }
};

template <class K,class E>
class binarysearchtree{
public:
    binarysearchtree(){ root = NULL; treesize=0;}
    bool empty(){ return treesize==0;}
    void insert(const E& thekey);
    void find(const E& thekey);
    void erase(const E& thekey);
    void find_rank(int t);
    void erase_rank(int t);
private:
    binarytreenode<K,E>* root;
    int treesize;
};

template<class K,class E>
void binarysearchtree<K,E>::insert(const E& thekey){
    binarytreenode<K,E> *p = root;
    binarytreenode<K,E> *pp = NULL;
    int num = 0;
    while(p!=NULL){
        pp = p;
        if(thekey<p->element.second){              //插入的元素比节点小，往左孩子走；大，往右孩子走
            num^=p->element.second;
            p=p->leftchild;
        }
        else if(thekey>p->element.second){
            num^=p->element.second;
            p=p->rightchild;
        }
        else{
            printf("0\n");
            return ;
        }
    }
    binarytreenode<K,E>* temp = new binarytreenode<K,E>(make_pair(0,thekey));
    if(root!=NULL){
        if(thekey<pp->element.second) pp->leftchild = temp;
        else pp->rightchild = temp;
    }
    else{
        root = temp;
    }
    printf("%d\n",num);
    treesize++;
    p = root;
    while(p->element.second!=thekey){           //跟新索引值，如果插入到左子树，索引值++
        if(thekey<p->element.second){
            p->element.first++;
            p=p->leftchild;
        }
        else if(thekey>p->element.second){
            p=p->rightchild;
        }
    }
}

template<class K,class E>
void binarysearchtree<K,E>::find(const E& thekey){
    binarytreenode<K,E> *p = root;
    int num=thekey;
    while(p!=NULL){
        if(thekey < p->element.second){
            num^=p->element.second;
            p=p->leftchild;
        }
        else if(thekey > p->element.second){
            num^=p->element.second;
            p=p->rightchild;
        }
        else{
            printf("%d\n",num);
            return ;
        }
    }
    printf("0\n");
}

template<class K,class E>
void binarysearchtree<K,E>::erase(const E& thekey){
    binarytreenode<K,E> *p = root;
    binarytreenode<K,E> *pp = NULL;
    int num=thekey;
    while(p!=NULL && p->element.second!=thekey){
        if(thekey<p->element.second){
            num^=p->element.second;
            p=p->leftchild;
        }
        else if(thekey>p->element.second){
            num^=p->element.second;
            p=p->rightchild;
        }
    }
    if(p==NULL) {
        printf("0\n");
        return ;
    }
    printf("%d\n",num);
    p = root;
    while(p!=NULL && p->element.second!=thekey){
        pp=p;
        if(thekey<p->element.second){
            p->element.first--;
            p=p->leftchild;
        }
        else if(thekey>p->element.second){
            p=p->rightchild;
        }
    }

    if(p->leftchild!=NULL && p->rightchild!=NULL){
        binarytreenode<K,E> *s = p->rightchild;
        binarytreenode<K,E> *ps = p;
        while(s->leftchild!=NULL){
            ps=s;
            s->element.first--;
            s=s->leftchild;
        }
        binarytreenode<K,E> *q = new binarytreenode<K,E> (make_pair(p->element.first,s->element.second),p->leftchild,p->rightchild);
        if(pp==NULL) root = q;
        else if(p==pp->leftchild){
            pp->leftchild = q;
        }
        else pp->rightchild = q;
        if(ps == p) pp=q;
        else pp=ps;        //pp为p的父节点
        delete p;
        p=s;       //p为要删去的节点，即右子树的最小元素
    }

    binarytreenode<K,E> *c;
    if(p->leftchild != NULL) c=p->leftchild;   
    else c=p->rightchild;                      
                                                         //p为右子树的最小元素，即c=NULL
    if(p==root) root=c;
    else{
        if(p==pp->leftchild){
            pp->leftchild = c;
        }
        else pp->rightchild = c;
    }
    delete p;
    treesize--;
}

template<class K,class E>
void binarysearchtree<K,E>::find_rank(int t){
    if(t>treesize || t<=0){
        printf("0\n");
        return ;
    }
    binarytreenode<K,E> *p = root;
    int num=0;
    int pre=0;
    while(p!=NULL){
        if(t < pre + p->element.first +1){          //根据索引值找到名次为t的节点
            num^=p->element.second;
            p=p->leftchild;
        }
        else if(t > pre + p->element.first +1){
            num^=p->element.second;
            pre+=p->element.first+1;
            p=p->rightchild;
        }
        else{
            num^=p->element.second;
            printf("%d\n",num);
            return ;
        }
    }
}

template<class K,class E>
void binarysearchtree<K,E>::erase_rank(int t){
    if(t>treesize || t<=0){
        printf("0\n");
        return ;
    }
    binarytreenode<K,E> *p = root;
    binarytreenode<K,E> *pp = NULL;
    int num=0;
    int pre=0;
    while(p!=NULL && t != pre + p->element.first +1){   //索引是左子树元素个数
        pp=p;
        if(t < pre + p->element.first +1){
            num^=p->element.second;
            p=p->leftchild;
        }
        else if(t > pre + p->element.first +1){     
            num^=p->element.second;
            pre+=p->element.first+1;
            p=p->rightchild;
        }
    }
    num^=p->element.second;
    printf("%d\n",num);
    E thekey = p->element.second;
    p = root;
    while(p!=NULL && p->element.second!=thekey){
        pp=p;
        if(thekey<p->element.second){
            p->element.first--;
            p=p->leftchild;
        }
        else if(thekey>p->element.second){
            p=p->rightchild;
        }
    }
    if(p->leftchild!=NULL && p->rightchild!=NULL){
        binarytreenode<K,E> *s = p->rightchild;
        binarytreenode<K,E> *ps = p;
        while(s->leftchild!=NULL){
            ps=s;
            s->element.first--;
            s=s->leftchild;
        }
        binarytreenode<K,E> *q = new binarytreenode<K,E> (make_pair(p->element.first,s->element.second),p->leftchild,p->rightchild);
        if(pp==NULL) root = q;
        else if(p==pp->leftchild){
            pp->leftchild = q;
        }
        else pp->rightchild = q;
        if(ps == p) pp=q;
        else pp=ps;        //pp为p的父节点
        delete p;
        p=s;       //p为要删去的节点，即右子树的最小元素
    }

    binarytreenode<K,E> *c;
    c=NULL;
    if(p->leftchild != NULL) c=p->leftchild;   
    else if(p->rightchild!=NULL) c=p->rightchild;                      
                                                         //p为右子树的最小元素，即c=NULL
    if(p==root) root=c;
    else{
        if(p==pp->leftchild){
            pp->leftchild = c;
        }
        else pp->rightchild = c;
    }
    delete p;
    treesize--;
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    scanf("%d",&n);
    binarysearchtree<int,int> tree;
    for(int i=0;i<n;i++){
        int temp,key;
        scanf("%d%d",&temp,&key);
        if(temp==0) tree.insert(key);
        if(temp==1) tree.find(key);
        if(temp==2) tree.erase(key);
        if(temp==3) tree.find_rank(key);
        if(temp==4) tree.erase_rank(key);
    }
}