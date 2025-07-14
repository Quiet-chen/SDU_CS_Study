#include<iostream>
#include<vector>
#include<queue>
#include<ctime>
#include<fstream>
using namespace std;
int pmax,pmin,d;//pmax到pmin的距离不超过d，cost是距离的一个函数，假设cost=距离，pmin=0，d=pmax
int p[20005];
int num=0;
int din[20005]={0};//入度
struct node{
    int v,p;
};
vector<pair<int,int>>g[20005];

void bianli(int u){
    for(auto vv:g[u]){
        if(p[u]-vv.second>=pmin){
            p[vv.first]=max(p[vv.first],p[u]-vv.second);//从一个点出发判断指向的另一点的p是否>=pmin，更新p值
        }
        else{
            p[u]=pmax;//如果p<pmin，说明u要放一个放大器，重新遍历u的邻边，更新它们的p值
            num++;
            bianli(u);
            return;
        }
    }
}

int main(){
    clock_t start,end;
    start=clock();
    freopen("filein.in","r",stdin);
    cin>>pmax;//pmin=0,d=pmax-pmin
    int n,m;
    cin>>n>>m;
    for(int i=0;i<m;i++){
        int u,v,c;
        cin>>u>>v>>c;
        g[u].push_back({v,c});
        din[v]++;
    }
    for(int i=0;i<=n;i++) p[i]=-1;
    queue<node>q;
    for(int i=1;i<=n;i++){
        if(din[i]==0){
            q.push((node){i,pmax});//把入度为零的点先放入队列
            p[i]=pmax;
        }
    }
    while(!q.empty()){
        node tmp=q.front();
        q.pop();
        int u=tmp.v;
        bianli(u);          //更新p
        for(auto vv:g[u]){
            din[vv.first]--;//更新入度
            if(din[vv.first]==0) q.push((node){vv.first,p[vv.first]});//入度为零的点放入队列
        }
    }
    cout<<num<<'\n';
    end=clock();
    printf( "%f s\n", (double)(end - start) / CLOCKS_PER_SEC );
}