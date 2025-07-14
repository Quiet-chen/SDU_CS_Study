/*
* Filename : ipc.h
* copyright : (C) 2006 by zhonghonglie
* Function : 声明 IPC 机制的函数原型和全局变量
*/
#ifndef IPC_H
#define IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#define BUFSZ 256

//建立或获取 ipc 的一组函数的原型说明
int get_ipc_id(char *proc_file,key_t key);
char *set_shm(key_t shm_key,int shm_num,int shm_flag); 
int set_msq(key_t msq_key,int msq_flag);
int set_sem(key_t sem_key,int sem_val,int sem_flag); 
int down(int sem_id);
int up(int sem_id);

/*信号量控制用的共同体*/
typedef union semuns {
    int val;
} Sem_uns;

/* 消 息 结 构 体 */
typedef struct msgbuf {
    long mtype; 
    char mtext[1];
} Msg_buf;

//生产消费者共享缓冲区即其有关的变量 
static key_t buff_key;
static int buff_num;
static char *buff_ptr;
//生产者放产品位置的共享指针
static key_t pput_key;
static int pput_num; 
static int *pput_ptr;
//消费者取产品位置的共享指针
static key_t cget_key;
static int cget_num;
static int *cget_ptr;
// 生产者有关的信号量
//缓冲区已满的信号量
static key_t full_key;
//保证存数的时候是互斥的信号量
static key_t mutex_key;
static int full_sem;
static int mutex_sem;

// 消费者有关的信号量
static key_t GlueTobacco_key;//有胶水和草
static key_t GluePaper_key;//有胶水和纸
static key_t PaperTobacco_key;//有纸和草
static int GlueTobacco_sem;
static int GluePaper_sem;
static int PaperTobacco_sem;

static int sem_val;
static int sem_flg;
static int shm_flg;
#endif /* IPC_H */