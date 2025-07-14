#include<iostream>
#include<string>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<queue>
using namespace std;

template<class T>
class losertree{   //最小输者树，小的赢，数组下标都从1开始
public:
    losertree(T* theplayer, int n);
    int getwin(){
        return tree[0];       //返回赢者，即最小的数的下标
    }
    int winner(int x, int y);
    int loser(int x, int y);
    void play(int p,int l,int r);
    void replay(int theplayer,T thevalue);//theplayer数组下标,thevalue要修改的新值
    ~losertree(){
        delete [] tree;
        delete [] thewin;
    }
private:
    int numofplayer; //比赛成员个数
    int* tree;       //内部节点，数组描述，存储输者下标
    int* thewin;   //每次赢的成员下标
    T* player;       //参加比赛的成员数组
    int lowext;      //竞赛树最底层的外部节点个数, 2*(n-s), 有的外部节点赢者直接晋级到上上层
    int offset;      //成员下标与对应内部节点相互转换的计算下标偏移量, 2*s-1
};

template<class T>
losertree<T>::losertree(T* theplayer,int n){ //theplayer为比赛成员，n为成员个数
    if(n<2) return;    //节点数必大于等于2
    numofplayer=n;
    player=theplayer;
    tree=new int [n+1];//从1到n
    thewin=new int [n+1];
    int s;
    for(s=1;2*s<=n-1;s*=2);//s=2^log(n-1)-1,最底层最左节点index，这样计算s更快
    lowext=2*(n-s); //lowext和offset的计算公式，不会推、硬记
    offset=2*s-1;
    for(int i=2;i<=lowext;i+=2) play((i+offset)/2,i-1,i);//从初始数组开始比，得到输者树最底层元素，i-1和i所以每次i+2，到lowext即遍历最底层对应的外部节点
    int t=0;
    if(n%2==1){ //奇数节点,则有一个节点单独和前面外部节点的赢者比，因为奇数节点的最后一个不产生最底层的赢者，所以赢者树总数为n-1
        play(n/2,thewin[n-1],lowext+1);//前面一组外部节点的赢者放在赢者数组的最后一个，与下一个单独的外部节点下标为lowext+1，比较之后赢者放在内部节点的倒数第二层，n为奇数，n/2=(n-1)/2，即n-1的赢者下标为n/2
        t=lowext+3;//把lowext下一个节点单独处理后，只剩下0个或偶数个剩下的外部节点
    }
    else{
        t=lowext+2;//t为要处理的剩下的外部节点的下一组的右成员下标
    }
    for(int i=t;i<=n;i+=2) play((i-lowext+n-1)/2,i-1,i);//剩下的外部节点，它们的赢者连晋两级，这个下标也不会推TwT
    tree[0]=thewin[1];
}

template<class T>
int losertree<T>::winner(int x,int y){ //小的赢，返回赢者在成员数组中的下标
    if(player[x]<=player[y]) return x;
    else return y;
}

template<class T>
int losertree<T>::loser(int x,int y){
    if(player[x]<=player[y]) return y;
    else return x;
}

template<class T>
void losertree<T>::play(int p,int l,int r){
    tree[p]=loser(l,r);
    thewin[p]=winner(l,r);
    while(p%2==1 && p>1){  //树下标为奇数，可以往上得出上层晋级者
        tree[p/2]=loser(thewin[p-1],thewin[p]);
        thewin[p/2]=winner(thewin[p-1],thewin[p]);
        p=p/2;
    }
}

template<class T>
void losertree<T>::replay(int theplayer, T thevalue){//某个成员值变了，重构树
    int n=numofplayer;
    if(theplayer<=0||theplayer>n) return; //下标合法
    player[theplayer]=thevalue;
    int treeindex,l,r;//treeindex为树中对应要修改的节点下标
    if(theplayer<=lowext){//修改的节点在竞赛树底层的外部节点
        treeindex=(offset+theplayer)/2;
        l=2*treeindex-offset;//l=theplayer，或者theplayer左边，看player奇偶，通过treeindex计算省略了讨论
        r=l+1;
    }
    else{
        treeindex=(theplayer-lowext+n-1)/2;//记
        if(2*treeindex==n-1){ //n为奇数，且是外部节点和内部底层最后一个赢者比
            l=thewin[2*treeindex];
            r=theplayer;
        }
        else{  
            l=2*treeindex-n+1+lowext;  //两个都是>lowext的成员
            r=l+1;
        }
    }
    if(theplayer==tree[0]){//重构的点是全赢者
        for(;treeindex>=1;treeindex/=2){
            int oldloser=tree[treeindex]; //保存一下之前的输者，与新值比较
            tree[treeindex]=loser(oldloser,theplayer);
            thewin[treeindex]=winner(oldloser,theplayer);
            theplayer=thewin[treeindex];  //theplayer保存每次赢者，往上比较
        }
    }
    else{
        tree[treeindex]=loser(l,r);
        thewin[treeindex]=winner(l,r);
        if(treeindex==n-1&&n%2==1){//treeindex是最后一层的最后一场比赛，且n是奇数
            treeindex/=2;
            tree[treeindex]=loser(thewin[n-1],lowext+1);  //外部节点和内部底层最后一个赢者比
            thewin[treeindex]=winner(thewin[n-1],lowext+1);
        }
        treeindex/=2;
        for(;treeindex>=1;treeindex/=2){  //竞赛树内部节点比较
            tree[treeindex]=loser(thewin[treeindex*2],thewin[treeindex*2+1]);
            thewin[treeindex]=winner(thewin[treeindex*2],thewin[treeindex*2+1]);
        }
    }
    tree[0]=thewin[1];
}

struct seq{  //顺串数结构体
    int num,key;  //顺串号，关键字
    bool operator<=(seq &p){   //num优先，比较顺串大小
        if(num<p.num) return true;
        else if(num==p.num){
            if(key<=p.key) return true;
        }
        return false;
    }
};

struct filedisk{ //大磁盘[文件]
    string path; //路径
    int nvisit,sumall,cachesize,numofdisk;//访问次数，数字总个数，缓冲区大小，生成文件个数
    int k;//要求归并数
};

void init(filedisk &fi){  //初始化外排序数据，即生成一个较长文件作为外排序初始数据
    cout<<"size of cache:"<<endl; //设置缓冲区大小
    cin>>fi.cachesize;
    cout<<"initial number of disk:"<<endl;  //初始数据个数
    cin>>fi.sumall;
    cout<<"number of merge:"<<endl;
    cin>>fi.k;
    fi.path="disk.txt";   //保存到disk.txt文件中
    fi.nvisit=0;      
    ofstream fileout(fi.path);
    if(!fileout.is_open()){   //确保文件正确打开
        cout<<"error open file"<<endl;
        exit(0);
    }
    srand((unsigned)time(NULL));  //生成随机数种子
    for(int i=0;i<fi.sumall;i++){ //生成随机数
        int x=rand()%1000+1;
        fileout<<x<<' ';          //输出到文件
    }
    fileout.close();
}

void make_seq(filedisk &fi, seq *theseq){  //生成最初归并串
    ifstream filein(fi.path);  //从初始大文件输入
    if(!filein.is_open()){
        cout<<"erroe open file"<<endl;
        exit(0);
    }
    for(int i=1;i<=fi.cachesize;i++){//把前size个数生成初始串
        filein>>theseq[i].key;
        theseq[i].num=1;  //设初始顺串号为1
        fi.nvisit++;  //每从文件输入一个数，文件访问次数++
    }
    losertree<seq> seqtree(theseq,fi.cachesize); //构建初始顺串输者树
    int nn=0;
    for(int i=1;i<=fi.sumall;i++){ //从大磁盘中不断取数
        if(!(filein>>nn)) nn=INT_MIN;//文件所有数都输入完了先把nn变为最小值
        else fi.nvisit++;            //输出一个数就要访问一次磁盘
        seq thewinner=theseq[seqtree.getwin()]; //找到最小值顺串下标
        seq newseq;
        newseq.key=nn;               //文件中下一个数代替最小顺串的位置
        if(nn>=thewinner.key) newseq.num=thewinner.num;  //如果新值大于之前的最小值，则新值对应的顺串号相同
        else{
            if(nn==INT_MIN){
                newseq.key=INT_MAX;
                newseq.num=INT_MAX; //把最大值放入输者树，使其最后一个赢，标记过程结束     
            }     
            else  newseq.num=thewinner.num+1; //否则新值顺串号+1
        }
        seqtree.replay(seqtree.getwin(),newseq); //新树重构
        string sdisk="disk"+to_string(thewinner.num)+".txt";
        fi.numofdisk=max(fi.numofdisk,thewinner.num);//更新磁盘块数
        ofstream fileout(sdisk,ios::app);//追加模式，在末尾添加
        fileout<<thewinner.key<<' ';     //输出最小值到对应顺串磁盘块里
        fileout.close();
        fi.nvisit++;                     //每次输出访问次数++
    }
    cout<<"the sum of seq:"<<fi.numofdisk<<endl;
}

void merge_k(filedisk &fi,int k,int nnn,int diskn){//k路归并, 文件起始号diskn，要输出的归并文件起始号
    string toname="disk"+to_string(nnn)+".txt";
    ofstream fileout(toname);
    queue<int> cache[k+2];
    if(k==1){
        string sfile="disk"+to_string(diskn)+".txt";
        ifstream filein(sfile);
        int temp;
        while(filein>>temp){
            for(int i=0;i<fi.cachesize;i++){
                fileout<<temp<<' '; 
                if(!(filein>>temp)) break; 
            }
            fi.nvisit+=2;
        }
        fileout.close();
        filein.close();
        return ;
    }
    int theplayer[k+5]; 
    int sp[k+5];               //存储每个小文件当前数字读取位置
    for(int i=1;i<=k;i++){     //初始化theplayer为各个顺串最小值
        string sfile="disk"+to_string(diskn+i-1)+".txt";
        ifstream filein(sfile);
        for(int j=0;j<fi.cachesize;j++){  //把k个缓冲区读满数据
            int t;
            filein>>t;
            cache[i].push(t);
        }        
        fi.nvisit++;
        theplayer[i]=cache[i].front();
        cache[i].pop();
        sp[i]=filein.tellg();             //记录读取位置
        filein.close();
    }
    losertree<int> disktree(theplayer,k); //新的磁盘数最小输者树
    while(1){  //直到赢者为正无穷合并结束
        int temp=disktree.getwin();  //temp为赢者下标，即对应顺串号
        int thewinner=theplayer[temp];
        if(thewinner==INT_MAX) break;
        if(cache[k+1].size()>=fi.cachesize){ //输出缓冲区满，把缓冲区数据输出到文件
            int t;
            while(!cache[k+1].empty()){
                t=cache[k+1].front();
                cache[k+1].pop();
                fileout<<t<<' ';
            }
            fi.nvisit++;
        }
        cache[k+1].push(thewinner);     //赢者输出到最终文件缓冲区
        int nn;
        if(cache[temp].empty()){        //对应文件的输入缓冲区空了，再次读取数据
            string name1="disk"+to_string(diskn+temp-1)+".txt"; 
            ifstream filein(name1); 
            filein.seekg(sp[temp]+1);//移动指针位置到sp之前存的位置的下一个，跳过空格
            for(int j=0;j<fi.cachesize;j++){
                int t;
                if(filein>>t){
                    cache[temp].push(t);
                }
                else{
                    cache[temp].push(INT_MAX); //下一位置为空，则该文件全部读取完毕，设最大值
                    break;
                }
            }
            fi.nvisit++;
            sp[temp]=filein.tellg();
            filein.close();
        }
        nn=cache[temp].front();  //新的代替赢者的即同一磁盘块缓冲区中的次小值
        cache[temp].pop();
        disktree.replay(temp,nn);
    }
    int t;
    if(!cache[k+1].empty()) fi.nvisit++;
    while(!cache[k+1].empty()){
        t=cache[k+1].front();
        cache[k+1].pop();
        fileout<<t<<' ';
    }
    fileout.close();
}

void merge_seq(filedisk &fi){   //合并顺串
    int nnn=fi.numofdisk+1;     //已有文件数
    int next=1;                 //要归并的起始位置
    while(next+fi.k<=nnn){      //k路归并
        merge_k(fi,fi.k,nnn,next);
        nnn++;
        next+=fi.k;
    }
    if(nnn-next>1) merge_k(fi,nnn-next,nnn,next); //不足k路且不是一路，归并
    cout<<"visit disk:"<<fi.nvisit<<endl;
}

int main(){
    filedisk thefile;
    init(thefile);
    seq theplayer[thefile.sumall+5];
    make_seq(thefile,theplayer);
    merge_seq(thefile);
}