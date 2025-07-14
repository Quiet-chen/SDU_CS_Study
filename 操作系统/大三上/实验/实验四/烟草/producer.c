#include "ipc.h"
int main(int argc, char *argv[])
{
	int rate;
	// 可在在命令行第一参数指定一个进程睡眠秒数,以调解进程执行速度
	if(argv[1] != NULL) 
		rate = atoi(argv[1]);
	else 
		rate = 3; // 不指定为 3 秒
	// 共享内存使用的变量
	buff_key = 101; // 缓冲区任给的键值
	buff_num = 4; // 缓冲区任给的长度
	pput_key = 102; // 生产者放产品指针的键值
	pput_num = 1; // 指针数
	shm_flg = IPC_CREAT | 0644; // 共享内存读写权限
	// 获取缓冲区使用的共享内存,buff_ptr 指向缓冲区首地址
	buff_ptr = (char*)set_shm(buff_key, buff_num, shm_flg);
	// 获取生产者放产品位置指针 pput_ptr
	pput_ptr = (int*)set_shm(pput_key, pput_num, shm_flg);
	// 信号量使用的变量
	full_key = 201;   //同步
	mutex_key = 202;  //互斥
	GlueTobacco_key = 203;
	GluePaper_key = 204;
	PaperTobacco_key = 205;
	sem_flg = IPC_CREAT | 0644;
	// 生产者同步信号灯初值设为缓冲区最大可用量
	sem_val = buff_num;
	// 获取full同步信号灯,引用标识存 full_sem
	full_sem = set_sem(full_key,sem_val,sem_flg);
	// 消费者初始无产品可取,同步信号灯初值设为 0
	sem_val = 0;
	// 获取消费者同步信号灯,引用标识存 GlueTobacco_sem
	GlueTobacco_sem = set_sem(GlueTobacco_key,sem_val,sem_flg);
	// 获取消费者同步信号灯,引用标识存 GluePaper_sem
	GluePaper_sem = set_sem(GluePaper_key,sem_val,sem_flg);
	// 获取消费者同步信号灯,引用标识存 PaperTobacco_sem
	PaperTobacco_sem = set_sem(PaperTobacco_key,sem_val,sem_flg);
	// 生产者互斥信号灯初值为 1
	sem_val = 1;
	// 获取生产者互斥信号灯,引用标识存 mutex_sem
	mutex_sem = set_sem(mutex_key,sem_val,sem_flg);
	//缓冲区的数组下标
   
	// 循环执行模拟生产者不断放产品
	int index=0;
	while(1)
	{
		//int count = rand() % 3;
        int count = index % 3;
         
		// 如果缓冲区满则生产者阻塞
		down(full_sem);
		// 如果另一生产者正在放产品,本生产者阻塞
		down(mutex_sem);
		// 用写一个材料的形式模拟生产者放产品,报告本进程号和放入的字符及存放的位置
		buff_ptr[*pput_ptr] = 'A'+ index;
		index++;
		sleep(rate);
		if(count == 0)
		{
			printf("第%d号进程放入胶水和烟草:%c 到 Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);	
		}
		else if(count == 1)
		{
			printf("第%d号进程放入胶水和纸:%c 到 Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);	
		}
		else if(count == 2)
		{
			printf("第%d号进程放入纸和烟草:%c 到 Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);	
		}
		// 存放位置循环下移
		*pput_ptr = (*pput_ptr+1) % buff_num;
		// 唤醒阻塞的生产者	
		up(mutex_sem);
		//唤醒对应的抽烟者
		if(count == 0)
		{
			up(GlueTobacco_sem);
		}
		else if(count == 1)
		{
			up(GluePaper_sem);
		}
		else if(count == 2)
		{
			up(PaperTobacco_sem);
		}
		
		
	}
	return EXIT_SUCCESS;
}