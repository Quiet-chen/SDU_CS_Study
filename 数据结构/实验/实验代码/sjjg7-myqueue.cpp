#include <iostream>
using namespace std;

template<class T>
class myqueue{
public:
    myqueue(int n=100);
    void init(int n);
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

template<class T>
void myqueue<T>::init(int n){    //按题目初始化队列
    for(int i=1;i<=n;i++){
        element[i-1]=i;
    }
    queuefront=queuelength-1;
    queueback=n-1;
    queuesize=n;
}

template <class T>
bool myqueue<T>::empty(){
    return queuefront == queueback;
}

template <class T>
void myqueue<T>::push(T& thelement){
    if(queuefront==(queueback+1)%queuelength){                    //若空间不够，重新进行动态分配
        queuelength*=2;                                //循环队列扩展空间写错了
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

int main(){
    int n;
    cin>>n;
    myqueue<int> a(n+5);
    a.init(n);
    while(a.size()>1){
        a.pop();
        int i=a.front();
        a.pop();
        a.push(i);
    }
    int ans=a.front();
    cout<<ans;
}

