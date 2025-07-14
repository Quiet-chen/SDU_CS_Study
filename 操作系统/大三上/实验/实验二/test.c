#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
void task1(int *); //线程 1 执行函数原型
void task2(int *); //线程 2 执行函数原型
void task3(int *); //线程 3 执行函数原型
int isstop=1;
int pipe1[2],pipe2[2],pipe3[2],pipe4[2]; //13是线程13通信，24是线程23通信
pthread_t thrd1,thrd2,thrd3; //存放第两个线程标识
int main(int argc,char *arg[])
{
    int ret;
    int num1,num2,num3;
    //使用 pipe()系统调用建立两个无名管道。建立不成功程序退出，执行终止
    if(pipe(pipe1) < 0){
        perror("pipe1 not create"); 
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe2) < 0)
    { 
        perror("pipe2 not create");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe3) < 0)
    { 
        perror("pipe3 not create");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe4) < 0)
    { 
        perror("pipe4 not create");
        exit(EXIT_FAILURE);
    }
    //使用 pthread_create 系统调用建立两个线程。建立不成功程序退出，执行终止
    num1 = 1 ;
    ret = pthread_create(&thrd1,NULL,(void *) task1,(void *) &num1); 
    if(ret){
        perror("pthread_create: task1"); 
        exit(EXIT_FAILURE);
    }
    num2 = 2 ;
    ret = pthread_create(&thrd2,NULL,(void *) task2,(void *) &num2); 
    if(ret){
        perror("pthread_create: task2"); 
        exit(EXIT_FAILURE);
    }
    num3 = 3 ;
    ret = pthread_create(&thrd3,NULL,(void *) task3,(void *) &num3); 
    if(ret){
        perror("pthread_create: task3"); 
        exit(EXIT_FAILURE);
    }
    
    //挂起当前线程，等待线程 thrd1 结束，并回收其资源
    pthread_join(thrd1,NULL);
    //挂起当前线程，等待线程 thrd2 结束，并回收其资源
    pthread_join(thrd2,NULL); 
    //挂起当前线程，等待线程 thrd3 结束，并回收其资源
    pthread_join(thrd3,NULL);
    //思考与测试：如果去掉上述两个 pthread_join 的函数调用，会出现什么现象
    exit(EXIT_SUCCESS);
}

void task1(int *num)//f(x)=f(x-1)*x
{
    int x=1;
    int n=1;
    int r;
    do{
        write(pipe1[1],&x,sizeof(int)); 
        printf("thread%d write: %d\n",*num,x);
        read(pipe3[0],&r,sizeof(int));
        n++; 
        x=n*x;
    }while(n<=10&&isstop);
    isstop=0;
    //读写完成后,关闭管道
    close(pipe1[1]);
    close(pipe3[0]);
}

void task2(int * num)//f(y)=f(y-1)+f(y-2)
{
    int x1=0;
    int x2=1;
    int x=1;
    int r;
    do{
        write(pipe2[1],&x,sizeof(int));
        printf("thread2 write: %d\n",x);
        read(pipe4[0],&r,sizeof(int));
        x=x1+x2;
        x1=x2;
        x2=x;
    }while(isstop);
    //读写完成后,关闭管道
    close(pipe2[1]);
    close(pipe4[0]);
}

void task3(int * num){//f(x,y)=f(x)+f(y)
    int x1;
    int x2;
    do{
        read(pipe1[0],&x1,sizeof(int)); 
        read(pipe2[0],&x2,sizeof(int));
        printf("thread3 read1: %d, read2: %d, f(x)+f(y)= %d\n",x1,x2,x1+x2);
        write(pipe3[1],&x1,sizeof(int));
        write(pipe4[1],&x2,sizeof(int));
    }while(isstop);
    //读写完成后,关闭管道
    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe3[1]);
    close(pipe4[1]);
}