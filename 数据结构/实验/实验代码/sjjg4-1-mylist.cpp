#include <iostream>
using namespace std;

template<class T>
struct chainnode{       //节点类
    T element;
    chainnode<T> *next;
    chainnode(){ }
    chainnode(const T& thelement){  //构造函数
        this->element=thelement;
    }
    chainnode(const T& element,chainnode<T>* next){
        this->element=element;
        this->next=next;
    }
};

template<class T>
class sortlist{                        //链表类
public:
    sortlist();
    sortlist(const sortlist<T>& thelist);
    void insert(int index,const T& thelement);
    void erase(const T& thelement);
    void reverse();
    void find(const T& thelement);

    class iterator{                     //链表成员类迭代器
    public:
        iterator(chainnode<T>* thenode){ node = thenode; }
        T& operator*() const { return node->element; }          //返回节点值
        T* operator->() const { return & node->element; }
        iterator& operator++(){                            //前++
            node = node->next; 
            return *this;
        }
        iterator operator++(int){                       //后++
            iterator last = *this;
            node=node->next;
            return last;
        }
        bool operator!=(const iterator theiter) const{
            return node!=theiter.node ;
        }
        bool operator==(const iterator theiter) const{
            return node==theiter.node;
        }
    private:
        chainnode<T>* node; 
    };

    iterator begin(){
        return iterator(firstnode);
    }
    iterator end(){
        return NULL;
    }
    ~sortlist(){
        while(firstnode!=NULL){
            chainnode<T>* temp=firstnode->next;
            delete firstnode;
            firstnode=temp;
        }
    }
private:
    chainnode<T>* firstnode;
    int size;
};

template<class T>
sortlist<T>::sortlist(){
    firstnode=NULL;
    size=0;
}

template<class T>
sortlist<T>::sortlist(const sortlist<T>& thelist){            //复制构造函数
    size=thelist.size;
    if(size==0){
        firstnode=NULL;
        return ;
    }
    chainnode<T>* temp = thelist.firstnode;          //temp指针指向被复制的链表
    firstnode = new chainnode<T> (temp->element);
    chainnode<T>* now = firstnode;                    //now指针指向新链表
    temp = temp->next;
    while(temp!=NULL){
        now -> next = new chainnode<T> (temp->element);   //now的next指向由temp的节点值构成的新节点
        now = now->next;
        temp = temp->next;
    }
    now->next = NULL;
}

template<class T>
void sortlist<T>::insert(int index,const T& thelement){  //index从0开始算
    size++;
    chainnode<T>* temp = firstnode;
    if(index==0){                                   //插入到头节点
        firstnode = new chainnode<T> (thelement);
        firstnode->next = temp;
        return ;
    }
    for(int i=0;i<index-1;i++){                 //中间或结尾插入
        temp=temp->next;
    }
    chainnode<T>* p = new chainnode<T> (thelement);
    p->next = temp->next;
    temp->next = p;
}

template<class T>
void sortlist<T>::erase(const T& thelement){    //删除节点
    chainnode<T>* temp = firstnode;
    size--;
    if(firstnode->element==thelement){     //删除的是头节点
        firstnode = firstnode->next;
        delete temp;
        return ;
    }
    while(temp->next!=NULL && temp->next->element!=thelement){
        temp=temp->next;
    }
    if(temp->next==NULL){               //没找到该节点
        cout<<-1<<endl;
        return ;
    }
    chainnode<T>* p = temp->next;
    temp->next = p->next;
    delete p;
}

template<class T>
void sortlist<T>::reverse(){               //链表颠倒
    chainnode<T>* pre = firstnode;          //前一节点
    chainnode<T>* temp = firstnode->next;   //后一节点
    firstnode = firstnode->next;
    pre->next=NULL;
    while(firstnode->next!=NULL){
        firstnode = firstnode->next;      //firstnode先移向temp的下一节点
        temp->next = pre;                 //temp的next指针指向前一节点
        pre=temp;
        temp=firstnode;
    }
    temp->next=pre;
}

template<class T>
void sortlist<T>::find(const T& thelement){            //查找
    chainnode<T>* temp = firstnode;
    int index=0;
    while(temp!=NULL && temp->element != thelement){
        temp=temp->next;
        index++;
    }
    if(temp==NULL){
        cout<<-1<<endl;
        return ;
    }
    cout<<index<<endl;
}

int main(){
    int n,q;
    cin>>n>>q;
    sortlist<int> thelist;
    for(int i=0;i<n;i++){
        int t;
        cin>>t;
        thelist.insert(i,t);
    }
    for(int i=0;i<q;i++){
        int p;
        cin>>p;
        if(p==1){
            int idx,val;
            cin>>idx>>val;
            thelist.insert(idx,val);
        }
        else if(p==2){
            int val;
            cin>>val;
            thelist.erase(val);
        }
        else if(p==3){
            thelist.reverse();
        }
        else if(p==4){
            int val;
            cin>>val;
            thelist.find(val);
        }
        else if(p==5){
            int index=0,sum=0;
            for(sortlist<int>::iterator i = thelist.begin();i!=thelist.end();i++){
                sum+=(index++)^(*i);
            }
            cout<<sum<<endl;
        }
    }
}