#include "ipc.h"
int main(int argc, char *argv[])
{
  int i, j = 0;
  int rate;
  Msg_buf msg_arg;
  //可在在命令行第一参数指定一个进程睡眠秒数,以调解进程执行速度
  if (argv[1] != NULL)
    rate = atoi(argv[1]);
  else
    rate = 3;
  //联系一个请求消息队列
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

  //进程调用msgsnd()给消息队列发送信息，或调用msgrcv()从消息队列接受消息。

  int pid1, pid2;
  pid1 = fork();
  if (pid1 == 0)
  {
    while (1)
    {
      msgctl(chair_quest_id, IPC_STAT, &chair_que);
      if (chair_que.msg_qnum == 0)
      {
        printf("%d号理发师睡眠\n", getpid()%10);
      }

      chair_quest_flg = 0;
      msgrcv(chair_quest_id, &msg_arg, sizeof(msg_arg), CHAIR, chair_quest_flg); // chair_quest_id所指代的消息队列取出消息，有则返回数据长度
      printf("%d号理发师为%d号顾客理发\n", getpid()%10, msg_arg.mid);               //用进程号区分三个理发师，用当前的缓冲区的id区分客户
      up(customer_sem);
      sleep(rate); //给理发师缓冲时间
      down(account_sem);
      printf("%d号理发师收取%d号顾客交费\n", getpid()%10, msg_arg.mid); //理完付钱
      up(account_sem);
    }
  }
  else
  {
    pid2 = fork();
    if (pid2 == 0)
    {
      while (1)
      {
        msgctl(chair_quest_id, IPC_STAT, &chair_que);
        if (chair_que.msg_qnum == 0)
        {
          printf("%d号理发师睡眠\n", getpid()%10);
        }

        chair_quest_flg = 0;
        msgrcv(chair_quest_id, &msg_arg, sizeof(msg_arg), CHAIR, chair_quest_flg); // chair_quest_id所指代的消息队列取出消息，有则返回数据长度
        printf("%d号理发师为%d号顾客理发\n", getpid()%10, msg_arg.mid);               //用进程号区分三个理发师，用当前的缓冲区的id区分客户
        up(customer_sem);
        sleep(rate); //给理发师缓冲时间
        down(account_sem);
        printf("%d号理发师收取%d号顾客交费\n", getpid()%10, msg_arg.mid); //理完付钱
        up(account_sem);
      }
    }
    else
    {
      while (1)
      {
        msgctl(chair_quest_id, IPC_STAT, &chair_que);
        if (chair_que.msg_qnum == 0)
        {
          printf("%d号理发师睡眠\n", getpid()%10);
        }

        chair_quest_flg = 0;
        msgrcv(chair_quest_id, &msg_arg, sizeof(msg_arg), CHAIR, chair_quest_flg); // chair_quest_id所指代的消息队列取出消息，有则返回数据长度
        printf("%d号理发师为%d号顾客理发\n",getpid()%10, msg_arg.mid);               //用进程号区分三个理发师，用当前的缓冲区的id区分客户
        up(customer_sem);
        sleep(rate); //给理发师缓冲时间
        down(account_sem);
        printf("%d号理发师收取%d号顾客交费\n", getpid()%10, msg_arg.mid); //理完付钱
        up(account_sem);
      }
    }
  }
  return EXIT_SUCCESS;
}
