#include<iostream>
#include<vector>
#include<queue>
#include<ctime>
using namespace std;
int pmax,pmin,d;//pmax到pmin的距离不超过d，cost是距离的一个函数，假设cost=距离，pmin=0，d=pmax
int p[10005];
int num=INT_MAX;
int n,m;
int vis[10005]={0};
int din[10005]={0};//入度
vector<pair<int,int>>g[10005];

bool check(){
    for(int u=1;u<=n;u++){
        for(auto vv:g[u]){
            if(vis[vv.first]) continue;
            if(p[u]-vv.second<pmin) return false;
            else{
                if(p[vv.first]!=pmax+1)  p[vv.first]=min(p[vv.first],p[u]-vv.second);
                else p[vv.first]=p[u]-vv.second;
            }
        }
    }
    return true;
}

int main(){
    clock_t start,end;
    start=clock();
    freopen("file.txt","r",stdin);
    cin>>pmax>>pmin;//pmin=0,d=pmax-pmin
    cin>>n>>m;
    for(int i=0;i<m;i++){
        int u,v,c;
        cin>>u>>v>>c;
        g[u].push_back({v,c});
        din[v]++;
    }
    for(int i=1;i<=n;i++){
        p[i]=pmax+1;
    }
    while(vis[n+1]==0){
        vis[1]++;
        for(int i=1;i<=n;i++){
            if(vis[i]>=2){
                vis[i]=0;vis[i+1]++;
            }
            else break;
        }
        int tmp=0;
        for(int i=1;i<=n;i++){
            if(vis[i]){
                p[i]=pmax;
                tmp++;
            }
        }
        if(check()) num=min(num,tmp);
    }
    cout<<num<<'\n';
    end=clock();
    printf( "%f s\n", (double)(end - start) / CLOCKS_PER_SEC );
}