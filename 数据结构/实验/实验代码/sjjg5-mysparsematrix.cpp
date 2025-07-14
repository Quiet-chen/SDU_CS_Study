#include <iostream>
using namespace std;

template<class T>
struct matrixterm{
    int row;
    int col;
    T value;
};

template <class T>
class myarray{
public:
    myarray(int capacity=20);
    void resetsize(int newsize);
    void copy(myarray<T> &b);
    void insert(T& newelement);     
    void set(int index,T& newelement);  
    void clear();
    int arraysize(){
        return size;
    }

    class iterator{                     
    public:
        iterator(T* theposition){ position=theposition; }
        T& operator*() const { return *position; }          
        T* operator->() const { return & *position; }
        iterator& operator++(){                            //前++
            ++position; 
            return *this;
        }
        iterator operator++(int){                       //后++
            iterator last = *this;
            ++position;
            return last;
        }
        iterator& operator--(){                            //前--
            --position; 
            return *this;
        }
        iterator operator--(int){                       //后--
            iterator last = *this;
            --position;
            return last;
        }
        bool operator!=(const iterator theiter) const{
            return position!=theiter.position ;
        }
        bool operator==(const iterator theiter) const{
            return position==theiter.position;
        }
    private:
        T* position; 
    };

    iterator begin(){
        return iterator(element);
    }
    iterator end(){
        return iterator(element+size);
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

template<class T>
void myarray<T>::resetsize(int nowsize){
    if(nowsize > length){                    //若空间不够，重新进行动态分配
        length=nowsize;
        T* temp = new T [length];
        for(int i=0;i<size;i++){
            temp[i]=element[i];
        }
        T* p =  element;
        element = temp;
        delete [] p;
    }
    size=nowsize;
}

template<class T>
void myarray<T>::copy(myarray<T> &b){
    length=b.length;
    size=b.size;
    T* p =  element;
    element = new T [length];
    for(int i=0;i<size;i++){
        element[i]=b.element[i];
    }
    delete [] p;
}

template <class T>
void myarray<T>::insert(T& newelement){   //在数组最后插入新元素
    if(size>=length){                    //若空间不够，重新进行动态分配
        length*=2;
        T* temp = new T [length];
        for(int i=0;i<size;i++){
            temp[i]=element[i];
        }
        T* p =  element;
        element = temp;
        delete [] p;
    }
    element[size++]=newelement;     //队尾插入
}

template<class T>
void myarray<T>::set(int index,T& newelement){
    element[index]=newelement;  
}

template<class T>
void myarray<T>::clear(){
    T* p =  element;
    element = new T [length];
    delete [] p;
    size=0;
}

template <class T>
myarray<T>::~myarray(){
    size=0,length=0;
    T* p =  element;
    element = NULL;
    delete [] p;
}


template<class T>
class mysparsematrix{
public:
    myarray<matrixterm<T>>terms;       //terms是公有成员，方便使用
    mysparsematrix(){}
    void set();
    void reset();
    void copy(mysparsematrix<T> &b);
    void add(mysparsematrix<T> &b);
    void transpose();
    void multipul(mysparsematrix<T> &b);
    void output();
    ~mysparsematrix(){ terms.~myarray(); rows=0; cols=0; }
private:
    int rows;
    int cols;
     
};

template<class T>
void mysparsematrix<T>::set(){  //稀疏矩阵输入
    terms.clear();
    cin>>rows>>cols;
    int t;
    cin>>t;
    for(int i=0;i<t;i++){
        matrixterm<T> temp;
        cin>>temp.row>>temp.col>>temp.value;
        terms.insert(temp);
    }
}

template<class T>
void mysparsematrix<T>::reset(){   //普通矩阵输入
    terms.clear();
    cin>>rows>>cols;
    for(int i=1;i<=rows;i++){
        for(int j=1;j<=cols;j++){
            int t;
            cin>>t;
            if(t!=0){
                matrixterm<T> temp;
                temp.row=i;
                temp.col=j;
                temp.value=t;
                terms.insert(temp);
            }
        }
    }
}

template <class T>
void mysparsematrix<T>::copy(mysparsematrix<T> &b){ //复制矩阵
    rows=b.rows;
    cols=b.cols;
    terms.copy(b.terms);
}

template <class T>
void mysparsematrix<T>::transpose(){
    mysparsematrix<T>b;
    b.cols=rows;
    b.rows=cols;
    b.terms.resetsize(terms.arraysize());
    int* colsize = new int [cols+1];  //每一列有多少个元素
    int* rownext = new int [cols+1];  //每一行第一个元素在第几个位子
    for(int i=0;i<=cols;i++) colsize[i]=0;
    for(typename myarray<matrixterm<T>>::iterator i=terms.begin();i!=terms.end();i++) colsize[(*i).col]++;
    rownext[1]=0;
    for(int i=2;i<=cols;i++) rownext[i]=colsize[i-1]+rownext[i-1];
    matrixterm<T> temp;
    for(typename myarray<matrixterm<T>>::iterator i=terms.begin();i!=terms.end();i++){
        int j = rownext[(*i).col]++;    //找到对应的位子，rownext++表示下一次这一排的元素该放的位子
        temp.col=(*i).row; 
        temp.row=(*i).col;
        temp.value=(*i).value;
        b.terms.set(j,temp);
    }
    this->copy(b);
}

template<class T>
void mysparsematrix<T>::add(mysparsematrix<T> &b){
    if(rows!=b.rows || cols!=b.cols){
        this->copy(b);
        cout<<-1<<endl;
        return ;
    }
    mysparsematrix<T>c;
    c.rows = rows;
    c.cols = cols;
    c.terms.clear();
    typename myarray<matrixterm<T>>::iterator ithis = terms.begin();
    typename myarray<matrixterm<T>>::iterator ib = b.terms.begin();
    while(ithis!=terms.end() && ib!=b.terms.end()){
        int thisindex = ((*ithis).row - 1) * cols + (*ithis).col;  //行列都是从1开始算
        int bindex = ((*ib).row - 1) * cols + (*ib).col;
        if(thisindex < bindex){
            c.terms.insert(*ithis);
            ithis++;
        }
        else if(thisindex == bindex){                          //只有位置相同的元素才相加，其他不变
            if((*ithis).value+(*ib).value != 0){
                matrixterm<T> temp;
                temp.row=(*ithis).row;
                temp.col=(*ithis).col;
                temp.value=(*ithis).value+(*ib).value;
                c.terms.insert(temp);
            }
            ithis++;
            ib++;
        }
        else{
            c.terms.insert(*ib);
            ib++;
        }
    }
    while(ithis!=terms.end()){
        c.terms.insert(*ithis);
        ithis++;
    }
    while(ib!=b.terms.end()){
        c.terms.insert(*ib);
        ib++;
    }
    this->copy(c);
}

template<class T>
void mysparsematrix<T>::multipul(mysparsematrix<T> &b){
    if(cols!=b.rows){
        cout<<-1<<endl;
        this->copy(b);
        return ;
    }
    mysparsematrix<T>c;
    c.rows = rows;
    c.cols = b.cols;
    c.terms.clear();
    b.transpose();               //转置之后方便操作
    typename myarray<matrixterm<T>>::iterator ithis = terms.begin();
    typename myarray<matrixterm<T>>::iterator ib = b.terms.begin();
    typename myarray<matrixterm<T>>::iterator p = terms.begin();
    for(int i=1;i<=rows;i++){
        p = ithis;                              //标记每一排的起始元组
        for(int j=1;j<=b.rows;j++){
            T sum=0;
            while(ithis!=terms.end() && ib!=b.terms.end() && ithis->row==i && ib->row==j){
                if(ithis->col==ib->col){                      //列相等的是对应的位子
                    sum+=ithis->value * ib->value;           //乘完加到sum里
                    ithis++;
                    ib++;
                }
                else if(ithis->col < ib->col){
                    ithis++;
                }
                else {
                    ib++;
                }
            }
            if(sum!=0){
                matrixterm<T> temp;
                temp.row=i;
                temp.col=j;
                temp.value=sum;
                c.terms.insert(temp);
            }
            if(j!=b.rows) ithis = p;        //一排乘完，ithis回到本行行首
        }
        ib = b.terms.begin();     //i一轮走完，要换下一排了，ib回到b矩阵的开头
    }
    this->copy(c);
}

template<class T>
void mysparsematrix<T>::output(){        //输出矩阵
    cout<<rows<<' '<<cols<<endl;
    typename myarray<matrixterm<T>>::iterator ithis = terms.begin();
    for(int i=1;i<=rows;i++){
        for(int j=1;j<=cols;j++){
            if(ithis!=terms.end() && (*ithis).row==i && (*ithis).col==j){
                cout<<(*ithis).value<<' ';
                ithis++;
            }
            else{
                cout<<0<<' ';
            }
        }
        cout<<endl;
    }
}

int main(){
    int n;
    cin>>n;
    mysparsematrix<int>P;
    for(int i=0;i<n;i++){
        int p;
        cin>>p;
        if(p==1){
            P.reset();
        }
        else if(p==2){
            mysparsematrix<int>b;
            b.set();
            P.multipul(b);
        }
        else if(p==3){
            mysparsematrix<int>b;
            b.set();
            P.add(b);
        }
        else if(p==4){
            P.output();
        }
        else if(p==5){
            P.transpose();
        }
    }
}