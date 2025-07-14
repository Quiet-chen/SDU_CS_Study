#include <iostream>
#include <iomanip>
using namespace std;

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
        return 0.0;
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

bool check(char top,char y){     //判断是否圧栈
    if(y=='('||top=='(') return true;
    if(y==')') return false;
    if((top=='+' || top=='-') && (y=='*' || y=='/')){
        return true;
    }
    return false;
}

template<class T, class M>
void tosum(mystack<T>&a, mystack<M>& b){     //进行二元运算
    double x = a.top();
    a.pop();
    double y = a.top();
    a.pop();
    char sign = b.top();
    b.pop();
    double sum=0;
    if(sign=='+') sum=x+y;
    if(sign=='-') sum=y-x;
    if(sign=='*') sum=x*y;
    if(sign=='/') sum=y/x;
    a.push(sum);
}

void calculate(){
    mystack<double> a;  //数字
    mystack<char> b;  //运算符
    char p;
    getchar();
    while(cin.peek()!='\n'){
        cin>>p;
        if(p<='9' && p>='0'){
            double num = p-'0';
            a.push(num);
        }
        else{
            if(b.empty() || check(b.top(),p)){   //如果运算符栈空或者新的运算符等级高，就圧栈
                b.push(p);
            }
            else{
                if(p==')'){
                    while(b.top()!='('){    //计算到栈顶是(，即括号运算结束
                        tosum(a,b);
                    }
                    b.pop();
                }
                else{
                    while(!b.empty() && !check(b.top(),p)){   //一直把比新运算符等级高或同级的运算都算完或者栈空才结束运算
                        tosum(a,b);
                    }
                    b.push(p);
                }
            }
        }
    }
    while(!b.empty()){
        tosum(a,b);
    }
    double sum = a.top();
    cout<<fixed<<setprecision(2)<<sum<<endl;
}

int main(){
    int n;
    cin>>n;
    for(int i=0;i<n;i++){
        calculate();
    }
}