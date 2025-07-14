#include <iostream>
using namespace std;

struct edge{
    int u,v,weight;
    bool operator>(const edge b) const{    //运算符重载
        return weight>b.weight;
    }
    bool operator>=(const edge b) const{    
        return weight>=b.weight;
    }
    bool operator<(const edge b) const{    
        return weight<b.weight;
    }
    bool operator<=(const edge b) const{    
        return weight<=b.weight;
    }
    bool operator==(const edge b) const{    
        return weight==b.weight;
    }
}edg[500005];

void quicksort(int leftend,int rightend){
    if(leftend>=rightend) return ;
    int leftcur=leftend;
    int rightcur=rightend+1;
    edge pivot=edg[leftend];
    while(1){
        do{
            leftcur++;
        }while(edg[leftcur]<pivot);
        do{
            rightcur--;
        }while(edg[rightcur]>pivot);
        if(leftcur>=rightcur) break;
        edge temp=edg[leftcur];
        edg[leftcur]=edg[rightcur];
        edg[rightcur]=temp;
    }
    edg[leftend]=edg[rightcur];
    edg[rightcur]=pivot;
    quicksort(leftend,rightcur-1);
    quicksort(rightcur+1,rightend);
}

void quicksort(int n){
    if(n<=0) return ;
    int max=-1;
    int index;
    for(int i=1;i<=n;i++){
        if(edg[i].weight>max){
            max=edg[i].weight;
            index=i;
        }
    }
    edge temp=edg[n];
    edg[n]=edg[index];
    edg[index]=temp;
    quicksort(1,n-1);
}

int father[5000005]={0};

int find(int i){
    if(i==father[i]) return i;
    int t = find(father[i]);
    father[i]=t;
    return t;
} 

void kruskal(int n,int e){
    edge temp;
    unsigned long long ans=0;
    int edge_tree=0;
    for(int i=1;i<=e;i++){
        temp = edg[i];
        int t1=find(temp.u);
        int t2=find(temp.v);
        if(t1!=t2){
            father[t1]=t2;
            ans+=temp.weight;
            edge_tree++;
        }
        if(edge_tree==n-1) break;
    }
    if(edge_tree==n-1) printf("%lu",ans);
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n,e;
    scanf("%d%d",&n,&e);
    for(int i=1;i<=e;i++){
        scanf("%d%d%d",&edg[i].u,&edg[i].v,&edg[i].weight);
    }
    for(int i=1;i<=n;i++){
        father[i]=i;
    }
    quicksort(e);
    kruskal(n,e);
}