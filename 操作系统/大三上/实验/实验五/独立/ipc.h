#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <unistd.h>
#define BUFSZ 256
#define MAXVAL 100
#define STRSIZ 8
#define SOFA 1
#define CHAIR 2
#define WAIT 3

/*信号灯控制用的共同体*/
typedef union semuns
{
    int val;
} Sem_uns;
/* 消息结构体*/
typedef struct msgbuf
{
    long mtype;
    int mid;
} Msg_buf;
static struct msqid_ds chair_que;
static struct msqid_ds sofa_que;
static struct msqid_ds wait_que;
static key_t customer_key;
static int customer_sem;
static key_t account_key;
static int account_sem;
static int sem_flg;
static int sem_val;

static int wait_quest_flg;
static key_t wait_quest_key;
static int wait_quest_id;

static int sofa_quest_flg;
static key_t sofa_quest_key;
static int sofa_quest_id;

static int chair_quest_flg;
static key_t chair_quest_key;
static int chair_quest_id;

int get_ipc_id(char *proc_file, key_t key);
char *set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_msq(key_t msq_key, int msq_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);
