#include<iostream>
#include<algorithm>
#include<queue>
#include<cstring>
#include<ctime>
using namespace std;
const int maxn=10005;
struct edge{
    int u,v,next,cost;
};
struct node{
    int p,num,tot,tag,father;
    bool operator < (const node& y) const{
        return tot>y.tot;
    }
};

int n,ans,pmax,pmin;
int din[maxn],vis[maxn],head[maxn],status[maxn],mp[maxn][maxn]={0};
int cnt,p[maxn],topo[maxn];
edge edg[maxn];

int tt=0;
void addedg(int u,int v,int w){
    edg[tt].u=u,edg[tt].v=v,edg[tt].cost=w;
    din[v]++;
    edg[tt].next=head[u];
    head[u]=tt;
    tt++;
    if(!mp[v][u]) mp[v][u]=w;
    mp[v][u]=min(mp[v][u],w);
}

void topusort(){
    priority_queue<int>q;
    cnt=0;
    for(int i=1;i<=n;i++){
        if(din[i]==0) q.push(-i);
    }
    while(!q.empty()){
        int u=-1*q.top();
        q.pop();
        topo[++cnt]=u;
        for(int i=head[u];i!=-1;i=edg[i].next){
            int v=edg[i].v;
            if(--din[v]==0) q.push(-v);
        }
    }
}

void fun(){
    priority_queue<node>q;
    node tree[maxn];
    int tag=0;
    node tmp;
    tmp.p=pmax;tmp.tot=0;tmp.num=1;tmp.tag=++tag;tmp.father=0;
    q.push(tmp);
    tree[tmp.tag]=tmp;
    while(!q.empty()){
        tmp=q.top();
        q.pop();
        if(tmp.num==n){
            ans=tmp.tot;
            break;
        }
        int v=topo[tmp.num+1];
        node grn=tree[tmp.tag];
        tmp.num++;tmp.p=-1;
        while(grn.tag){
            if(mp[v][topo[grn.num]]){
                tmp.p=max(tmp.p,tree[grn.tag].p-mp[v][topo[grn.num]]);
            }
            grn=tree[grn.father];
        }
        tmp.father=tmp.tag;
        tmp.tag=++tag;
        bool cc=0;
        for(int i=head[v];i!=-1;i=edg[i].next){
            int w=edg[i].cost;
            if(tmp.p-w<0){
                cc=1;
                break;
            }
        }
        if(tmp.p>=0&&cc==0){//不用放大器
            q.push(tmp);
            tree[tmp.tag]=tmp;
            tmp.tag=++tag;
        }
        tmp.tot++;tmp.p=pmax;tree[tmp.tag]=tmp;
        q.push(tmp);
    }
}

int main(){
    clock_t start,end;
    freopen("filein.in","r",stdin);
    start=clock();
    tt=0;
    ans=maxn;
    memset(p,0,sizeof(p));
    memset(din,0,sizeof(din));
    memset(vis,0,sizeof(vis));
    memset(status,0,sizeof(status));
    for(int i=1;i<=maxn;i++) head[i]=-1;
    int m;
    cin>>pmax>>n>>m;
    start=clock();
    for(int i=1;i<=m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        addedg(u,v,w);
    }
    topusort();
    fun();
    cout<<ans;
    end=clock();
    printf( "\n%f s\n", (double)(end - start) / CLOCKS_PER_SEC );
}