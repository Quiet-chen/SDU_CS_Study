#include <iostream>
#include <utility>
using namespace std;

template<class K,class E>
struct pairnode{       //节点类
    pair<K,E> element;
    pairnode<K,E> *next;
    pairnode(){ }
    pairnode(const pair<K,E>& thelement){  //构造函数
        this->element=thelement;
    }
    pairnode(const pair<K,E>& element,pairnode<K,E>* next){
        this->element=element;
        this->next=next;
    }
};

template<class K,class E>
class sortlist{                        //链表类
public:
    sortlist(){
        firstnode=NULL;
        size=0;
    }
    void insert(const pair<K,E>& thelement);
    void erase(const K& thekey);
    void find(const K& thekey);
    ~sortlist(){
        while(firstnode!=NULL){
            pairnode<K,E>* temp=firstnode->next;
            delete firstnode;
            firstnode=temp;
        }
    }
private:
    pairnode<K,E>* firstnode;
    int size;
};

template<class K,class E>
void sortlist<K,E>::insert(const pair<K,E>& thelement){
    pairnode<K,E> *p = firstnode,                                //tp为p的前一节点
                  *tp = NULL;
    while(p!=NULL && p->element.first < thelement.first){
        tp=p;
        p=p->next;
    }
    if(p!=NULL && p->element.first==thelement.first){
        cout<<"Existed"<<endl;
        return ;
    }
    pairnode<K,E>* newnode = new pairnode<K,E>(make_pair(thelement.first,0),p);
    if(tp==NULL) firstnode = newnode;
    else tp->next=newnode;
    size++;
}

template<class K,class E>
void sortlist<K,E>::erase(const K& thekey){
    pairnode<K,E> *p = firstnode,
                  *tp = NULL;
    while(p!=NULL && p->element.first < thekey){
        tp=p;
        p=p->next;
    }
    if(p!=NULL && p->element.first==thekey){
        if(tp==NULL) firstnode = p->next;
        else tp->next = p->next;  
        delete p;
        size--;
        cout<<size<<endl;
    }
    else{
        cout<<"Delete Failed"<<endl;
    }
}

template<class K,class E>
void sortlist<K,E>::find(const K& thekey){
    pairnode<K,E> *p = firstnode,
                  *tp = NULL;
    while(p!=NULL && p->element.first < thekey){
        tp=p;
        p=p->next;
    }
    if(p!=NULL && p->element.first==thekey){
        cout<<size<<endl;
    }
    else{
        cout<<"Not Found"<<endl;
    }
}

template<class K,class E>
class listhash{
public:
    listhash(int n){
        divisor = n;
        table = new sortlist<K,E> [n];
    }
    void find(const K& thekey){
        table[thekey%divisor].find(thekey);
    }
    void insert(const pair<K,E>& thelement){
        table[thelement.first%divisor].insert(thelement);
    }
    void erase(const K& thekey){
        table[thekey%divisor].erase(thekey);
    }
private:
    sortlist<K,E>* table;
    int divisor;
};

int main(){
    int d,m;
    cin>>d>>m;
    listhash<int,int> T(d);
    for(int i=0;i<m;i++){
        int p,x;
        cin>>p>>x;
        if(p==0){
            pair<int,int> temp = make_pair(x,0);
            T.insert(temp);
        } 
        if(p==1) T.find(x); 
        if(p==2) T.erase(x);
    }
}
