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

int n,ans,pmax;
int pmin=0;
int din[maxn],vis[maxn],head[maxn],status[maxn];
int cnt,p[maxn],topo[maxn];
edge edg[maxn];
bool flag;

int tt=0;
void addedg(int u,int v,int w){
    edg[tt].u=u,edg[tt].v=v,edg[tt].cost=w;
    din[v]++;
    edg[tt].next=head[u];
    head[u]=tt;
    tt++;
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

void dfs(int x){
    if(!flag) return ;
    vis[x]=1;
    for(int i=head[x];i!=-1;i=edg[i].next){
        int v=edg[i].v,w=edg[i].cost;
        if(p[x]>=w){
            if(vis[v]) continue;
            vis[v]=1;
            dfs(v);
        }
        else{
            flag=0;
            return ;
        }
    }
}

void judge(){  //判断这种情况的每个点的p是否>pmin,如果都大于，dfs修改p值
    status[1]=1;
    for(int i=1;i<=n;i++){
        vis[i]=0;
        if(status[i]) p[i]=pmax;
        else p[i]=-1;
    }
    for(int i=1;i<=cnt;i++){
        int u=topo[i];
        for(int j=head[u];j!=-1;j=edg[j].next){
            int v=edg[j].v,w=edg[j].cost;
            p[v]=max(p[v],p[u]-w);
        }
    }
    flag=1;
    for(int i=1;i<=n;i++){
        if(p[i]<pmin){
            flag=0;
            return ;
        }
    }
    dfs(1);
}



void meiju(int x,int tot){  //枚举所有情况，进行递归
    if(tot>ans) return;
    if(x>n){
        judge();
        if(flag) ans=min(ans,tot);
        return;
    }
    status[x]=1;
    meiju(x+1,tot+1);
    status[x]=0;
    meiju(x+1,tot);
}

int main(){
    clock_t start,end;
    freopen("filein.in","r",stdin);
    start=clock();
    ans=maxn;
    memset(p,0,sizeof(p));
    memset(din,0,sizeof(din));
    memset(vis,0,sizeof(vis));
    memset(status,0,sizeof(status));
    for(int i=1;i<=maxn;i++) head[i]=-1;
    int m;
    cin>>pmax>>n>>m;
    for(int i=1;i<=m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        addedg(u,v,w);
    }
    topusort();
    meiju(2,0);
    cout<<ans;
    end=clock();
    printf( "\n%f s\n", (double)(end - start) / CLOCKS_PER_SEC );
}