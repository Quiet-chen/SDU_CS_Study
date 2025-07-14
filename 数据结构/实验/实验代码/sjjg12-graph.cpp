#include <iostream>
using namespace std;

int num=0;

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
    void insert(const T& thelement);
    void erase(const T& thelement);
    ~sortlist(){
        while(firstnode!=NULL){
            chainnode<T>* temp=firstnode->next;
            delete firstnode;
            firstnode=temp;
        }
    }
    chainnode<T>* firstnode;
private:
    int size;
};

template<class T>
sortlist<T>::sortlist(){
    firstnode=NULL;
    size=0;
}

template<class T>
void sortlist<T>::insert(const T& thelement){  
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
void sortlist<T>::erase(const T& thelement){    //删除节点
    if(firstnode==NULL) return ;
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
        return ;
    }
    chainnode<T>* p = temp->next;
    temp->next = p->next;
    delete p;
}

template<class T>
class linkgraph{
public:
    linkgraph(){ row = new sortlist<T> [100005]; }
    void insert(T u,T v);
    void erase(T u,T v);
    void bfs(int v,int reach[],int label);
    T length_bfs(int v,int reach[],int label);
    void dfs(int v,int reach[],int label);
    void n_dfs(int v,int reach[],int label);
    void length_dfs(int v,int reach[],int label);
    void minroad(int v,int reach[],int reach2[],int label);
private:
    sortlist<T>* row;
};

template<class T>
void linkgraph<T>::insert(T u,T v){
    row[u].insert(v);
    row[v].insert(u);
}

template<class T>
void linkgraph<T>::erase(T u,T v){
    row[u].erase(v);
    row[v].erase(u);
}

template<class T>
void linkgraph<T>::bfs(int v,int reach[],int label){
    myqueue<T>q(100005);
    reach[v]=label;
    q.push(v);
    while(!q.empty()){  
        int w = q.front();
        cout<<w<<' ';
        q.pop();
        for(chainnode<int>* u = row[w].firstnode; u!=NULL; u = u->next){  //bfs把一个节点连接的所有节点都放入队列，以此类推
            if(reach[u->element]==0){
                q.push(u->element);                      
                reach[u->element]=label;
            }
        }
    }
}

template<class T>
T linkgraph<T>::length_bfs(int v,int reach[],int label){
    int num=1;
    myqueue<T>q(100005);
    reach[v]=label;
    q.push(v);
    while(!q.empty()){
        int w = q.front();
        
        q.pop();
        for(chainnode<int>* u = row[w].firstnode; u!=NULL; u = u->next){
            if(reach[u->element]==0){
                q.push(u->element);
                num++;
                reach[u->element]=label;
            }
        }
    }
    return num;
}

template<class T>
void linkgraph<T>::dfs(int v,int reach[],int label){
    reach[v]=label;
    cout<<v<<' ';
    chainnode<T>* u = row[v].firstnode;
    while(u != NULL){
        if(reach[u->element]==0){
            dfs(u->element,reach,label);               //不断访问相邻的节点直到没有未被访问过的节点为止
        }
        u=u->next;
    }
}

template<class T>
void linkgraph<T>::n_dfs(int v,int reach[],int label){
    reach[v]=label;
    chainnode<T>* u = row[v].firstnode;
    while(u != NULL){
        if(reach[u->element]==0){
            n_dfs(u->element,reach,label);
        }
        u=u->next;
    }
}

template<class T>
void linkgraph<T>::length_dfs(int v,int reach[],int label){
    reach[v]=label;
    chainnode<T>* u = row[v].firstnode;
    while(u != NULL){
        if(reach[u->element]==0){
            num++;
            length_dfs(u->element,reach,label);
        }
        u=u->next;
    }
}

template<class T>
void linkgraph<T>::minroad(int v,int reach[],int reach2[],int label){
    myqueue<T>q(100005);
    reach[v]=label;
    q.push(v);
    while(!q.empty()){
        int w = q.front();
        q.pop();
        for(chainnode<int>* u = row[w].firstnode;u!=NULL; u = u->next){
            if(reach[u->element]==0){
                q.push(u->element);
                reach[u->element]=label;
                reach2[u->element]=reach2[w]+1;            //最小路即bfs的基础上，定义一个reach2，值为前一层元素的值+1
            }
        }
    }
}

int main(){
    int reach[100005];
    int reach2[100005];
    int n,m,s,t;
    cin>>n>>m>>s>>t;
    linkgraph<int> G;
    for(int i=0;i<m;i++){
        int p,u,v;
        cin>>p>>u>>v;
        if(p==0 && u!=v) G.insert(u,v);
        if(p==1 && u!=v) G.erase(u,v);
    }

    num=0;
    for(int i=0;i<=n;i++) reach[i]=0;
    for(int i=1;i<=n;i++){
        if(reach[i]==0){
            num++;
            G.n_dfs(i,reach,s);
        }
    }
    cout<<num<<endl;

    for(int i=0;i<=n;i++) reach[i]=0;
    for(int i=1;i<=n;i++){
        if(reach[i]==0){
            cout<<i<<' ';
            G.n_dfs(i,reach,s);
        }
    }
    cout<<endl;

    for(int i=0;i<=n;i++) reach[i]=0;
    num=1;
    G.length_dfs(s,reach,s);
    cout<<num<<endl;

    for(int i=0;i<=n;i++) reach[i]=0;
    G.dfs(s,reach,s);
    cout<<endl;

    for(int i=0;i<=n;i++) reach[i]=0;
    num = G.length_bfs(t,reach,t);
    cout<<num<<endl;

    for(int i=0;i<=n;i++) reach[i]=0;
    G.bfs(t,reach,t);
    cout<<endl;

    for(int i=0;i<=n;i++) reach[i]=0;
    for(int i=0;i<=n;i++) reach2[i]=0;
    G.minroad(s,reach,reach2,s);
    if(s==t) cout<<0<<endl;
    else if(reach2[t]==0) cout<<-1<<endl;
    else cout<<reach2[t]<<endl;
}