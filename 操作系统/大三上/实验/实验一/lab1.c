#include"test.h"
int main(int argc, char *argv[]){
  //如果在命令行上没输入子进程要执行的命令
  //则执行缺省的命令

  int i;
  int pid1,pid2;//存放子进程号
  int status1,status2;//存放子进程返回状态
  char *args[]={"/bin/ls","-a",NULL};//子进程1要缺省执行的命令
  char *args2[]={"/bin/ps","-a",NULL};//子进程2要缺省执行的命令

  signal(SIGINT,&sigcat);//注册一个本进程处理键盘中断的函数
  perror("SIGINT");//如果系统调用signal成功执行，输出“SIGINT”，否则，
                    //输出“SIGINT”及出错原因
  
  while(1){
    pid1=fork();//建立子进程1
    if(pid1>0) pid2=fork();//建立子进程2

    if(pid1<0||pid2<0)//建立子进程失败？
    {
        printf("Create Process fail!\n");
        exit(EXIT_FAILURE);
    }

    if(pid1==0)//子进程1执行代码段
    {
        //报告父子进程进程号
        printf("I am Child process1 %d. My father is %d\n",getpid(),getppid());
        //getpid返回当前进程的进程号，getppid返回当前进程的父进程号
        printf("waiting child2.\n");

        pause(); //等待子进程2结束 

        printf("child2 done, %d child1 will Running: ",getpid()); 
        //执行缺省的命令，ls -a
        for(i=0; args[i] != NULL; i++) 
            printf("%s ",args[i]); printf("\n");
        //装入并执行新的程序
        status1 = execve(args[0],args,NULL);
    }
    else if(pid2==0)//子进程2执行的代码段
    {
        //报告父子进程进程号
        printf("I am Child process2 %d. My father is %d\n",getpid(),getppid());
        for(i=0; args2[i] != NULL; i++) 
            printf("%s ",args2[i]); printf("\n");
        
        //装入并执行新的程序
        status2 = execve(args2[0],args2,NULL);
    }
    else{ //父进程执行代码
        waitpid(pid2,&status2,0); //等待子进程2结束
        waitpid(pid1,&status1,0); //等待子进程1结束
        sleep(3);
    }
    
  }
  return EXIT_SUCCESS;
}