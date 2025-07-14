#include <iostream>
#include <utility>
using namespace std;

template<class K,class E>
class hashtable{
public:
    hashtable(int thedivisor);
    int search(const K& thekey);
    bool checknull(int b){
        return table[b]==NULL;
    }
    void insert(const K& thekey);
    void erase( K& thekey);
private:
    pair<const K,E>** table;
    int size;
    int divisor;
};

template<class K,class E>
hashtable<K,E>::hashtable(int thedivisor){     //初始化
    divisor=thedivisor;
    size=0;
    table = new pair<const K,E>* [divisor];
    for(int i=0;i<divisor;i++){
        table[i]=NULL;
    }
}

template<class K,class E>
int hashtable<K,E>::search(const K& thekey){    //查找
    int temp = thekey % divisor;
    int i=temp;
    do{
        if(table[i]==NULL||table[i]->first==thekey){      //table[i]为空即找不到，返回空位置的下标
            return i;
        }
        i=(i+1)%divisor;
    }while(i!=temp);
    return -1;                          //-1为表满
}

template<class K,class E>
void hashtable<K,E>::insert(const K& thekey){
    int b = search(thekey);                       
    if(b==-1){
        return ;
    }
    if(table[b]==NULL){                               //table[b]空，即插入这个位置
        table[b]=new pair<const K,E> (thekey,0);
        cout<<b<<endl;
        size++;
    }
    else if(table[b]->first==thekey){
        cout<<"Existed"<<endl;
    }
}

template<class K,class E>
void hashtable<K,E>::erase( K& thekey){
    int b = search(thekey);
    if(b==-1||table[b]==NULL){            //b为-1，即表满且没找到该元素；b为空位置下标，也是找不到
        cout<<"Not Found"<<endl;
        return ;
    }
    else{
        size--;
        int num=0;
        int pre=b;
        int i=(b+1)%divisor;
        int m=0;
        while(i!=b){
            if(table[i]!=NULL) m = (int)table[i]->first % divisor;  //m为该元素本应在的位置
            if(table[i]==NULL){                                    //为空 退出循环
                table[pre]=NULL;
                break;
            }
            else if(table[i]!=NULL && ((pre < i && i < m) || (i < m && m <= pre) || (m <= pre && pre < i) )){   
                //有三种情况，都可以把table[i]移到table[pre]，因为表是循环插入的， pre i m, i m pre , m pre i，即m pre i 的循环顺序
                table[pre]=table[i];
                pre=i;
                num++;
            }
            i=(i+1)%divisor;
        }
        cout<<num<<endl;
    }
}

int main(){
    int d,m;
    cin>>d>>m;
    hashtable<int,int> T(d);
    for(int i=0;i<m;i++){
        int p,x;
        cin>>p>>x;
        if(p==0) T.insert(x);
        if(p==1){
            int b = T.search(x); 
            if(b==-1||T.checknull(b)){    //b为-1或者table[b]为空都是找不到的情况
                cout<<-1<<endl;
            }
            else{
                cout<<b<<endl;
            }
        } 
        if(p==2) T.erase(x);
    }
}
