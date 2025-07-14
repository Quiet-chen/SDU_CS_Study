#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<sstream>
using namespace std;
typedef long long ll;

struct node{
    node* parent;   //父节点
    map<string, node*> child;//孩子节点
    int filetype;   //文件类型，0是文件，1是目录
    string filename;//名
    int childnum;
    ll filesize;   //该文件大小

    ll all_size;   //配额
    ll son_used;   //孩子大小

    node(int type, string fname, ll size=0){
        filetype=type;
        filesize=size;
        parent=NULL;
        child.clear();
        filename=fname;
        all_size=size;
        son_used=0;
    }
    bool preadd(ll size){  //预分配
        if(all_size<=0||son_used+size>all_size) return false;
        return true;
    }
    void addsize(ll size){
        son_used+=size;
    }
    bool setsize(ll size){
        if(filetype&&size>=son_used){
            all_size=size;
            return true;
        }
        return false;
    }
};

class catalogtree{
public:
    catalogtree(ll size){
        root=new node(1,"root",size);
        root->all_size=size;
        cur=root;
    }
    void save(){
        auto st=cout.rdbuf();
        string fileto="mls.txt";
        ofstream fileout(fileto);
        fileout<<"+"<<root->filename<<'\n';
        cout.rdbuf(fileout.rdbuf());
        dir1(1,root);
        cout.rdbuf(st);
    }
    void load();
    void read_mulu(ifstream &filein, int num,int n);
    void dir(){
        cout<<'+'<<cur->filename<<'\n';
        dir1(1,cur);
    }
    void dir1(int n,node* root1);//写出当前目录所有目录和文件
    void cd();  //写出当前目录的绝对路径
    void cd_back();//返回上一级
    string cd_change(string str);//更改当前目录为路径str
    void mkdir(string str,ll size);//创建子目录
    void mkfile(string str,ll size);//创建子文件
    void deletestr(string str);//删除str文件或目录
private:
    node* root;
    node* cur;
};

void catalogtree::load(){
    ifstream filein("load.txt");  //load文件中每一行有名字 子目录项数目 配额大小
    string temp;
    char c;
    filein>>c;
    filein>>temp;
    root->filename=temp;
    cur=root;
    int num,size;
    filein>>num>>size;
    cur->childnum=num;
    cur->filetype=1;
    cur->all_size=size;
    read_mulu(filein,num,1);
}

void catalogtree::read_mulu(ifstream &filein, int num, int n){
    char c;
    for(int i=0;i<num;i++){
        filein>>c;
        string name;int nn;ll ss;
        node* cc=new node(0,name,ss);
        if(c=='+'){
            filein>>name>>nn>>ss;
            cc->filetype=1;
            cc->filename=name;
            cc->all_size=ss;
            cc->parent=cur;
            cc->childnum=nn;
            cur->child.insert({name,cc});
            cur=cc;
            read_mulu(filein,nn,++n);
            cur=cur->parent;
            n--;
        }
        else{
            string name2;
            filein>>name2>>ss;
            name=c+name2;
            cc->parent=cur;
            cc->filename=name;
            cc->all_size=ss;
            cur->child.insert({name,cc});
        }
        cur->addsize(cc->son_used);
    }
}

void catalogtree::dir1(int n,node*root1){
    if(root1==NULL) return;
    auto ip=root1->child.begin();
    for(;ip!=root1->child.end();ip++){
        for(int j=0;j<n;j++) cout<<' ';
        if(ip->second->filetype){
            cout<<'+'<<ip->first<<'\n';
            dir1(++n,ip->second);
            n--;
        }
        else{
            cout<<ip->first<<'\n';
        } 
    }
}

void catalogtree::cd(){
    vector<string>str;
    node* pre=cur;
    while(pre!=root){
        str.push_back(pre->filename);
        pre=pre->parent;
    }
    cout<<'/'<<root->filename;
    for(int i=str.size()-1;i>=0;i--){
        cout<<'/'<<str[i];
    }
    cout<<'\n';
}

void catalogtree::cd_back(){
    cur=cur->parent;
}

string catalogtree::cd_change(string str){   //对某路径str，若路径正确，返回空，若路径中有找不到的目录名，cur变为缺失名的前一项，返回缺失值
    vector<string>path;
    stringstream ss(str);
    string temp;
    while(getline(ss,temp,'/')){
        path.push_back(temp);
    }
    if(path[0]!=root->filename){  //相对路径
        if(path[0]==".."){ //回到上级
            for(int i=0;i<path.size();i++){
                if(path[i]==".."){
                    cur=cur->parent;
                }
                else{
                    auto ip=cur->child.find(path[i]);
                    if(ip!=cur->child.end()) cur=ip->second;
                    else{
                        cout<<"not find "<<path[i]<<'\n';
                        return path[i];
                    }
                }
            }
        }
        else{  //去下一级
            for(int i=0;i<path.size();i++){
                auto ip=cur->child.find(path[i]);
                if(ip!=cur->child.end()) cur=ip->second;
                else{
                    cout<<"not find "<<path[i]<<'\n';
                    return path[i];
                }
            }
        }
    }
    else{               //绝对路径
        cur=root;
        for(int i=1;i<path.size();i++){
            auto ip=cur->child.find(path[i]);
            if(ip!=cur->child.end()) cur=ip->second;
            else{
                cout<<"not find "<<path[i]<<'\n';
                return path[i];
            }
        }
    }
    cout<<"find"<<'\n';
    return "";
}

void catalogtree::mkdir(string str,ll size=10){
    string thename=cd_change(str);
    if(cur->filetype==0||thename==""){
        cout<<"same name or cur filetype not catalog"<<endl; //无重名且插入到的文件时目录
        return;
    }
    auto ip=cur->child.find(thename); 
    if(ip==cur->child.end()){
        node* temp=new node(1,thename,size); //文件没有重名
        temp->parent=cur;
        temp->setsize(size);
        node* t=cur;
        int check=0;
        while(t!=root){
            if(t->preadd(size)==false){  //判断配额大小够不够
                check=1;
                break;
            }
            t=t->parent;
        }
        if(t->preadd(size)==false) check=1;
        if(check){
            cout<<"size full"<<endl;
        }
        else{
            node* tempt=cur;            //插入到目录下，并修改子目录项总大小
            while(tempt!=root){
                temp->addsize(size);
                tempt=tempt->parent;
            }
            root->addsize(size);
            temp->parent=cur;
            cur->child.insert({thename,temp}); 
            cout<<"succeed"<<endl;
        }
    }
    else{
        cout<<"same name"<<endl;
    }
}

void catalogtree::mkfile(string str,ll size=10){
    string thename=cd_change(str);
    if(cur->filetype==0){
        cout<<"same name or cur filetype not catalog"<<endl;
        return ;
    }
    auto ip=cur->child.find(thename);
    if(ip==cur->child.end()){
        node* temp=new node(0,thename,size);
        temp->parent=cur;
        node* t=cur;
        int check=0;
        while(t!=root){
            if(t->preadd(size)==false){
                check=1;
                break;
            }
            t=t->parent;
        }
        if(t->preadd(size)==false) check=1;
        if(check){
            cout<<"size full"<<endl;
        }
        else{
            node* tempt=cur;
            while(tempt!=root){
                temp->addsize(size);
                tempt=tempt->parent;
            }
            root->addsize(size);
            temp->parent=cur;
            cur->child.insert({thename,temp}); 
            cout<<"succeed"<<endl;
        }
    }
    else{
        cout<<"same name"<<endl;
    }
}

void catalogtree::deletestr(string str){
    string sss=cd_change(str);
    if(sss==""){
        node* pre=cur->parent;
        auto id=pre->child.find(cur->filename);
        pre->child.erase(id);
        cout<<"succeed"<<endl;
        cur=pre;
    }
    else{
        cout<<"faild"<<endl;
    }
}

int main(){
    catalogtree mulushu(100);
    mulushu.load();
    mulushu.save();
    string p;
    while(cin>>p){
        if(p=="cd"){
            if(cin.peek()!='\n'){
                cin>>p;
                if(p==".."){
                    mulushu.cd_back();
                }
                else{
                    mulushu.cd_change(p);
                }
            }
            else{
                mulushu.cd();
            }
        }
        if(p=="dir") mulushu.dir();
        if(p=="mkdir"){
            cin>>p;
            ll ss;
            cin>>ss;
            mulushu.mkdir(p,ss);
        }
        if(p=="mkfile"){
            cin>>p;
            ll ss;
            cin>>ss;
            mulushu.mkfile(p,ss);
        }
        if(p=="delete"){
            cin>>p;
            mulushu.deletestr(p);
        }
        cout<<"save? y or n"<<'\n';
        char c;
        cin>>c;
        if(c=='y') mulushu.save();
    }
}