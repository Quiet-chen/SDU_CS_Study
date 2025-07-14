#include<iostream>
using namespace std;
int n;
int a[15]={0};
int sum=0;
int ans=0;

void swap(int& x,int& y){
    int temp=x;
    x=y;
    y=temp;
}//交换函数

void fun(int i){
    if(i==n-1){//一种可能性排列完成，计算结果
        for(int p=0;p<n;p++){
            sum+=(p+1)^a[p];
        }
        ans|=sum;
        sum=0;
    }
    else{
        for(int j=i;j<n;j++){
            swap(a[j],a[i]);//后面每个数都有可能放在该位置上
            fun(i+1);
            swap(a[j],a[i]);
        }
    }
}

int main(){
    cin>>n;
    for(int i=0;i<n;i++){
        cin>>a[i];
    }
    fun(0);
    cout<<ans;
}
