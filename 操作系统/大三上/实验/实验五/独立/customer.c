#include "ipc.h"
int main(int argc, char *argv[])
{
  int i = 0;
  int rate;
  Msg_buf msg_arg;
  //可在在命令行第一参数指定一个进程睡眠秒数,以调解进程执行速度
  if (argv[1] != NULL)
    rate = atoi(argv[1]);
  else
    rate = 3;
  //联系一个请求消息队列,Is waitting room not full?
  wait_quest_flg = IPC_CREAT | 0644;
  wait_quest_key = 101;
  wait_quest_id = set_msq(wait_quest_key, wait_quest_flg);
  //联系一个请求消息队列,Is sofa empty/not full?
  sofa_quest_flg = IPC_CREAT | 0644;
  sofa_quest_key = 102;
  sofa_quest_id = set_msq(sofa_quest_key, sofa_quest_flg);

  chair_quest_flg = IPC_CREAT | 0644;
  chair_quest_key = 103;
  chair_quest_id = set_msq(chair_quest_key, chair_quest_flg);
  customer_key = 301;
  account_key = 302;
  sem_flg = IPC_CREAT | 0644;
  sem_val = 0;
  customer_sem = set_sem(customer_key, sem_val, sem_flg);
  sem_val = 1;
  account_sem = set_sem(account_key, sem_val, sem_flg);

  // int sofa_count = 0;
  // int wait_count = 0;
  // int chair_count = 0;

  while (1)
  {
    sleep(rate);
    msgctl(chair_quest_id, IPC_STAT, &chair_que);

    if (chair_que.msg_qnum < 3)
    {
      if (msgrcv(sofa_quest_id, &msg_arg, sizeof(msg_arg), SOFA, IPC_NOWAIT) >= 0)
      {
        msg_arg.mtype = CHAIR;
        chair_quest_flg = IPC_NOWAIT;
        printf("%d号顾客从沙发坐入理发椅\n", msg_arg.mid);
        msgsnd(chair_quest_id, &msg_arg, sizeof(msg_arg), chair_quest_flg);
      }
      else
      {
        msg_arg.mtype = CHAIR;
        msg_arg.mid = i;
        i++;
        chair_quest_flg = IPC_NOWAIT;
        printf("%d号新顾客直接坐入理发椅\n", msg_arg.mid);
        msgsnd(chair_quest_id, &msg_arg, sizeof(msg_arg), chair_quest_flg);
      }
    }
    else
    {
      msgctl(sofa_quest_id, IPC_STAT, &sofa_que);
      if (sofa_que.msg_qnum < 4)
      {
        //非阻塞方式接收消息
        if (msgrcv(wait_quest_id, &msg_arg, sizeof(msg_arg), WAIT, IPC_NOWAIT) >= 0)
        {
          msg_arg.mtype = SOFA;
          sofa_quest_flg = IPC_NOWAIT;
          printf("%d号顾客从等候室坐入沙发\n", msg_arg.mid);
          msgsnd(sofa_quest_id, &msg_arg, sizeof(msg_arg), sofa_quest_flg);
          
        }
        else
        {
          msg_arg.mtype = SOFA;
          msg_arg.mid = i;
          i++;
          printf("%d号新顾客直接坐入沙发\n", msg_arg.mid);
          sofa_quest_flg = IPC_NOWAIT;
          msgsnd(sofa_quest_id, &msg_arg, sizeof(msg_arg), sofa_quest_flg);
        }
      }
      else
      {
        msgctl(wait_quest_id, IPC_STAT, &wait_que);
        if (wait_que.msg_qnum < 13)
        {

          msg_arg.mtype = WAIT;
          msg_arg.mid = i++;
          printf("沙发坐满，%d号新顾客在等候室等候\n", msg_arg.mid);
          wait_quest_flg = IPC_NOWAIT;
          msgsnd(wait_quest_id, &msg_arg, sizeof(msg_arg), wait_quest_flg);
        }
        else
        {
          printf("等候室满，%d新顾客没有进入理发店\n", i);
          i++;
          down(customer_sem);
        }
      }
    }
  }
  return EXIT_SUCCESS;
}
