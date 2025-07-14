#include <iostream>
#include <cmath>
#include<cstdlib>
#include<time.h>
using namespace std;

template<class K,class E>
struct skipnode{
    typedef pair<K,E> pairtype;
    pairtype element;
    skipnode<K,E> **next;
    skipnode(const pairtype& thepair,int size):element(thepair){
        next=new skipnode<K,E> * [size];
    }
};

template<class K,class E>
class skiplist{
public:
    skiplist(K largekey,K headkey,int maxpairs,float prob);
    pair<K,E>* find(const K& thekey);
    int level(){
        int lev=0;
        while(rand()<=cutoff) lev++;          //随机数小于cutoff，级数++，即插入的节点的最高级++，直到随机数大于cutoff为止
        return lev<=maxlevel ? lev:maxlevel;   //级数不能超过maxlevel
    }
    skipnode<K,E>* search(const K& thekey);
    int insert(const pair<K,E>& thepair);
    int erase(const K thekey);
    void show();
    void strictskip();
    void strict(skipnode<K,E>* thebegin,skipnode<K,E>* theend,int thesize,int thelevel);
private:
    float cutoff;
    int levels;    //总共几级
    int dsize;     //节点数
    int maxlevel;  //最高级数
    K tailkey;
    skipnode<K,E>* headernode;
    skipnode<K,E>* tailnode;
    skipnode<K,E>** last;
};

template<class K,class E>
skiplist<K,E>::skiplist(K largekey,K headkey,int maxpairs,float prob){
    cutoff=prob*RAND_MAX;
    maxlevel=(int)ceil(logf((float)maxpairs)/logf(1/prob))-1;        //级数上限log n/log(1/p)取上整
    levels=0;
    dsize=2;
    tailkey=largekey;

    pair<K,E> tailpair;
    tailpair.first=tailkey;
    headernode=new skipnode<K,E> (make_pair(headkey,0),maxlevel+1);   //初始化头节点和尾节点，分别为最小值和最大值
    tailnode=new skipnode<K,E> (make_pair(tailkey,0),0);
    last=new skipnode<K,E> *[maxlevel+1];
    for(int i=0;i<=maxlevel;i++){
        headernode->next[i]=tailnode;  //将所有头节点指向尾节点
    }
}

template<class K,class E>
pair<K,E>* skiplist<K,E>::find(const K& thekey){
    int nodenum=0;
    if(thekey>=tailkey) return NULL;          //查找值要比tailkey小
    skipnode<K,E>* beforenode=headernode;
    for(int i=levels;i>=0;i--){
        cout<<beforenode->element.first<<' ';  //输出查找节点路径
        while(beforenode->next[i]->element.first<thekey){  //如果当前节点的next节点比查找值小，则去同级的下一节点，若比查找值大，则去看下一级的next节点，直到级数为0
            beforenode=beforenode->next[i];
            cout<<beforenode->element.first<<' ';
            nodenum++;
        }
        nodenum++;
    }
    cout<<"nodenum is "<<nodenum<<endl;
    if(beforenode->next[0]->element.first==thekey) return &beforenode->next[0]->element;//在循环时beforenode指向比thekey小的前一节点，故看next节点是否会等于thekey，等于则找到，不等则找不到
    return NULL;
}

template<class K,class E>
skipnode<K,E>* skiplist<K,E>::search(const K& thekey){
    skipnode<K,E>* beforenode=headernode;
    for(int i=levels;i>=0;i--){
        while(beforenode->next[i]->element.first<thekey){  //同find
            beforenode=beforenode->next[i];
        }
        last[i]=beforenode;    //标记每一级thekey的前一节点，以便后续的查找和删除操作
    }
    return beforenode->next[0];
}

template<class K,class E>
int skiplist<K,E>::insert(const pair<K,E>& thepair){
    int nodenum=0;
    if(thepair.first>=tailkey) return 0;

    int thekey=thepair.first;
    skipnode<K,E>* beforenode=headernode;
    for(int i=levels;i>=0;i--){
        while(beforenode->next[i]->element.first<thekey){  //即search过程
            beforenode=beforenode->next[i];
            nodenum++;
        }
        last[i]=beforenode;
    }
    skipnode<K,E>* thenode=beforenode->next[0];

    if(thenode->element.first==thepair.first){   //待插入值已存在，则退出函数
        thenode->element.second=thepair.second;
        return nodenum;
    }

    int thelevel=level();   //计算待插入值能有多少级
    if(thelevel>levels){         //如果级数比目前跳表的最高级都高，则待插入值的级数为跳表levels+1，否则会有多余累赘的空间
        thelevel=++levels;
        last[thelevel]=headernode;
    }

    skipnode<K,E>* newnode=new skipnode<K,E>(thepair,thelevel+1);
    for(int i=0;i<=thelevel;i++){
        newnode->next[i]=last[i]->next[i];   //插入到每一级的last和last的next之间
        last[i]->next[i]=newnode;
        nodenum++;
    }

    dsize++;
    return nodenum;
}

template<class K,class E>
int skiplist<K,E>::erase(const K thekey){
    int nodenum=0;
    if(thekey>=tailkey){
        cout<<"not found"<<endl;
        return 0;
    }
    skipnode<K,E>* beforenode=headernode;
    for(int i=levels;i>=0;i--){
        while(beforenode->next[i]->element.first<thekey){  //同search
            beforenode=beforenode->next[i];
            nodenum++;
        }
        nodenum++;
        last[i]=beforenode;
    }
    skipnode<K,E>* thenode=beforenode->next[0];
    if(thenode->element.first!=thekey){
        cout<<"not found"<<endl;
        return nodenum;
    }

    for(int i=0;i<=levels&&last[i]->next[i]==thenode;i++){  //若跳表中存在待删除节点，则遍历每一级的last->next，与thenode比较，是待删除节点就删除
        last[i]->next[i]=thenode->next[i];
        nodenum++;
    }
    while(levels>0&&headernode->next[levels]==tailnode) levels--; //如果最高级只剩头尾节点则将最高级删除
    delete thenode;
    dsize--;
    return nodenum;
}

template<class K,class E>
void skiplist<K,E>::show(){
    for(int i=0;i<=levels;i++){         //从下往上遍历每一级跳表进行输出
        skipnode<K,E>* beforenode=headernode;
        while(beforenode!=tailnode){
            cout<<beforenode->element.first<<' ';
            beforenode=beforenode->next[i];
        }
        cout<<tailnode->element.first<<endl;
    }
}

template<class K,class E>
void skiplist<K,E>::strict(skipnode<K,E>* thebegin,skipnode<K,E>* theend,int thesize,int thelevel){
    if(thesize<=2||thelevel<=0) return;
    int mid=thesize/2;  
    skipnode<K,E>* thenode = thebegin;
    for(int i=0;i<mid;i++){
        thenode=thenode->next[0];     //找到中间节点
    }
    thebegin->next[thelevel]=thenode;   //将头节点与中间节点相连
    thenode->next[thelevel]=theend;     //尾节点与中间节点相连
    strict(thebegin,thenode,mid+1,thelevel-1);     //将头-中间看成一段下一级的新的链表调用函数
    strict(thenode,theend,thesize-mid,thelevel-1);  //将中间-尾看成下一级另一段新的链表调用函数
}

template<class K,class E>
void skiplist<K,E>::strictskip(){
    int l=floor(log(dsize)/log(2));    //计算严格链表的最高级数log dsize
    levels=l-1;                        //levels从0开始计数
    strict(headernode,tailnode,dsize,levels);
}

int main(){
    int n;
    cin>>n;
    int key;
    skiplist<int,int>myskip(1e9,1,2000,0.5);//最小1，最大1e9，最多2000个节点，p值为0.5
    int sum=0;
    int nn=n;
    srand((unsigned)time(NULL));
    while(n){
        key = rand();                   //生成n个随机数插入跳表
        sum+=myskip.insert(make_pair(key,0));
        n--;
    }
    int ave = sum/nn;
    cout<<"sum of node vist: "<<sum<<endl;    //计算跳表插入平均访问节点
    cout<<"average node visit number: "<<ave<<endl;
    sum=0;
    myskip.show();
    cout<<"strictskip is:"<<endl;
    myskip.strictskip();                //将跳表整理成严格跳表
    myskip.show();
    cout<<"insert:"<<endl;
    cin>>key;
    myskip.insert(make_pair(key,0));     //插入数到严格跳表中
    myskip.show();
    cout<<"delete:"<<endl;             //删除某数
    cin>>key;
    sum=myskip.erase(key);
    cout<<"node visit sum: "<<sum<<endl;
    myskip.show();
    cout<<"you want to find:"<<endl;         //查找
    cin>>key;
    pair<int,int>* ans = myskip.find(key);
    if(ans==NULL) cout<<"not find"<<endl;
    else cout<<"find "<<ans->first<<endl;
    cout<<"strictskip is:"<<endl;           //最后再次整理为严格跳表
    myskip.strictskip();
    myskip.show();
}