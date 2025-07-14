#include <iostream>
using namespace std;

struct telephone{
    string name;       //姓名
    string tele;       //电话
    string clas;       //班级
    int domi;          //宿舍
    bool operator==(const telephone b) const{    //判等运算符重载
        if(this->name==b.name) return true;
        else return false;
    }
};

template <class T>
class myarray{
public:
    myarray(int capacity);
    void insert(T& newelement);       
    void erase(T thelement);
    void find(T thelement);
    void change(T thelement,int p,string t);
    void output(string t);
    int arraysize(){
        return size;
    }
    ~myarray();
private:
    int length;
    T* element;
    int size;
};

template <class T>
myarray<T>::myarray(int capacity){   //构造函数
    element = new T[capacity];
    length=capacity;
    size=0;
}

template <class T>
void myarray<T>::insert(T& newelement){   //在数组最后插入新元素
    if(size==length){                    //若空间不够，重新进行动态分配
        length*=2;
        T* temp = new T [length];
        for(int i=0;i<size;i++){
            temp[i]=element[i];
        }
        element=temp;
    }
    element[size++]=newelement;     //队尾插入
}

template <class T>
void myarray<T>::erase(T thelement){     //删除
    int i=0;
    bool check = false;         //标记是否找到元素
    for(;i<size;i++){
        if(element[i]==thelement){
            check=true;          
            break;   //找到要找的元素
        }
    }
    for(int j=i;j<size-1;j++){
        element[j]=element[j+1];       //删除该元素，后面的元素依次前移
    }
    if(check) size--;    
}

template<class T>
void myarray<T>::find(T thelement){    //查找
    for(int i=0;i<size;i++){
        if(element[i]==thelement){   //找到输出1
            cout<<1<<endl;
            return ;
        }
    }
    cout<<0<<endl;
}

void change_address(telephone &temp,int p,string t){   //结构体的修改
    if(p==1) temp.tele=t;
    if(p==2) temp.clas=t;
    if(p==3) temp.domi=atoi(t.c_str());
}

template <class T>
void myarray<T>::change(T thelement,int p,string t){   //对对应元素进行修改
    for(int i=0;i<size;i++){
        if(element[i]==thelement){
            change_address(element[i],p,t);
            break;
        }
    }
}

void sum_domi(telephone* temp,int leng,string t){   //计算异或和
    int sum=0;
    for(int i=0;i<leng;i++){
        if(temp[i].clas==t) sum^=temp[i].domi;
    }
    cout<<sum<<endl;
}

template <class T>
void myarray<T>::output(string t){
    sum_domi(element,this->arraysize(),t);
}

template <class T>
myarray<T>::~myarray(){
    delete [] element;
}

int main(){
    int n;
    cin>>n;
    myarray<telephone>address(100);
    for(int i=0;i<n;i++){
        int m;
        cin>>m;
        if(m==0){
            telephone temp;
            cin>>temp.name>>temp.tele>>temp.clas>>temp.domi;
            address.insert(temp);
        }
        else if(m==1){
            telephone temp;
            cin>>temp.name;
            address.erase(temp);
        }
        else if(m==2){
            telephone temp;
            int p;
            string t;
            cin>>temp.name>>p>>t;
            address.change(temp,p,t);
        }   
        else if(m==3){
            telephone temp;
            cin>>temp.name;
            address.find(temp);
        }
        else{
            string t;
            cin>>t;
            address.output(t);
        }
    }
}