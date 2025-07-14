#include <iostream>
using namespace std;
int n;
int ans=0;
int ans1=0;
int b[25]={0};//用1或0代表子集中是否有这个数
int c[25]={0};//存子集
void fun(int a[],int i){
	if(i==n+1){//从1到n都由1或0表示完了
		int num=1;
		for(int j=1;j<=n;j++){
			if(b[j]==1){
				c[num++]=a[j];//b数组为1表示子串中有这个数存到c数组里
			}
		}
		ans1=0;
		for(int j=1;j<num;j++){
			ans1+=j*c[j];//计算价值
		}
		ans^=ans1;
	}
	else{
		b[i]=1;//每个数字都有两种可能，分别递归
		fun(a,i+1);
		b[i]=0;
		fun(a,i+1);
	}
}
int main(){
	cin>>n;
	int a[25]={0};
	for(int i=1;i<=n;i++) cin>>a[i];
	fun(a,1);
	cout<<ans;
}












