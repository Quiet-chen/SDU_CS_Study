#include <iostream>
using namespace std;

template<class T>
struct chainnode{
    T element;
    chainnode<T> *next;
    chainnode(){ }
    chainnode(const T& thelement){
        this->element=thelement;
    }
    chainnode(const T& element,chainnode<T>* next){
        this->element=element;
        this->next=next;
    }
};

template<class T>
class sortlist{
public:
    sortlist();
    sortlist(const sortlist<T>& thelist);
    void insert(int index,const T& thelement);
    void erase(const T& thelement);
    void reverse();
    void find(const T& thelement);
    void merge(const sortlist<T>& b);
    void sort_insert(T thelement);
    void merge(sortlist<T>& a,sortlist<T>& b);
    chainnode<T>* head(){
        return firstnode;
    }

    class iterator{
    public:
        iterator(chainnode<T>* thenode){ node = thenode; }
        T& operator*() const { return node->element; }
        T* operator->() const { return & node->element; }
        iterator& operator++(){
            node = node->next; 
            return *this;
        }
        iterator operator++(int){ 
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
sortlist<T>::sortlist(const sortlist<T>& thelist){
    size=thelist.size;
    if(size==0){
        firstnode=NULL;
        return ;
    }
    chainnode<T>* temp = thelist.firstnode;
    firstnode = new chainnode<T> (temp->element);
    chainnode<T>* now = firstnode;
    temp = temp->next;
    while(temp!=NULL){
        now -> next = new chainnode<T> (temp->element);
        now = now->next;
        temp = temp->next;
    }
    now->next = NULL;
}

template<class T>
void sortlist<T>::insert(int index,const T& thelement){  //index从0开始算
    size++;
    chainnode<T>* temp = firstnode;
    if(index==0){
        firstnode = new chainnode<T> (thelement);
        firstnode->next = temp;
        return ;
    }
    for(int i=0;i<index-1;i++){
        temp=temp->next;
    }
    chainnode<T>* p = new chainnode<T> (thelement);
    p->next = temp->next;
    temp->next = p;
}

template<class T>
void sortlist<T>::erase(const T& thelement){
    chainnode<T>* temp = firstnode;
    size--;
    if(firstnode->element==thelement){
        firstnode = firstnode->next;
        delete temp;
        return ;
    }
    while(temp->next!=NULL && temp->next->element!=thelement){
        temp=temp->next;
    }
    if(temp->next==NULL){
        cout<<-1<<endl;
        return ;
    }
    chainnode<T>* p = temp->next;
    temp->next = p->next;
    delete p;
}

template<class T>
void sortlist<T>::reverse(){
    chainnode<T>* pre = firstnode;
    chainnode<T>* temp = firstnode->next;
    firstnode = firstnode->next;
    pre->next=NULL;
    while(firstnode->next!=NULL){
        firstnode = firstnode->next;
        temp->next = pre;
        pre=temp;
        temp=firstnode;
    }
    temp->next=pre;
}

template<class T>
void sortlist<T>::find(const T& thelement){
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

template<class T>
void sortlist<T>::sort_insert(T thelement){
    size++;
    chainnode<T>* temp = firstnode;
    if(firstnode==NULL || thelement < temp->element){
        firstnode = new chainnode<T> (thelement);
        firstnode->next = temp;
        return ;
    }
    while(temp->next!=NULL && thelement>temp->next->element){
        temp=temp->next;
    }
    chainnode<T>* p = new chainnode<T> (thelement);
    p->next = temp->next;
    temp->next = p;
}

template<class T>
void sortlist<T>::merge(sortlist<T>& a,sortlist<T>& b){
    chainnode<T>* temp_a = a.firstnode;
    chainnode<T>* temp_b = b.firstnode;
    int num=0;
    while(temp_a!=NULL && temp_b!=NULL){
        if(temp_a->element<temp_b->element){
            insert(num,temp_a->element);
            temp_a=temp_a->next;
            num++;
        }
        else{
            insert(num,temp_b->element);
            temp_b=temp_b->next;
            num++;
        }
    }
    while(temp_a!=NULL){
        insert(num,temp_a->element);
        temp_a=temp_a->next;
        num++;
    }
    while(temp_b!=NULL){
        insert(num,temp_b->element);
        temp_b=temp_b->next;
        num++;
    }
}

int main(){
    int n,m;
    cin>>n>>m;

    sortlist<int> list_n;
    sortlist<int> list_m;

    for(int i=0;i<n;i++) {
        int p;
        cin>>p;
        list_n.sort_insert(p);
    }
    for(int i=0;i<m;i++){
        int p;
        cin>>p;
        list_m.sort_insert(p);
    }

    int index=0,sum=0;
    for(sortlist<int>::iterator i = list_n.begin();i!=list_n.end();i++){
        sum+=(index++)^(*i);
    }
    cout<<sum<<endl; 

    index=0,sum=0;
    for(sortlist<int>::iterator i = list_m.begin();i!=list_m.end();i++){
        sum+=(index++)^(*i);
    }
    cout<<sum<<endl;

    sortlist<int> list_merge;
    list_merge.merge(list_n,list_m);

    index=0,sum=0;
    for(sortlist<int>::iterator i = list_merge.begin();i!=list_merge.end();i++){
        sum+=(index++)^(*i);
    }
    cout<<sum<<endl;
}