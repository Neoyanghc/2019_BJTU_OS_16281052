#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
//用于控制互斥访问M0~M3
sem_t *mutex0,*mutex1,*mutex2,*mutex3;
//用于控制同步访问M0~M3
sem_t *empty0,*empty1,*empty2,*empty3;
sem_t *full0,*full1,*full2,*full3;
//buffer
char M0[3]="111";
char M1[3]="000";
char M2[2]="00";
char M3[2]="00";
//进程1
void *process1(void *arg)
{
	int i=20;
	while(i--)
	{
		/* 从M0中取出消息 */
		/* sem_wait(mutex0); */
		sem_wait(full0);
		/* printf("信箱M0中的消息为"); */
		printf("进程0从信箱M0中取出一条消息\t");
		/* sem_post(mutex0); */
		sem_post(empty0);
		/* 向M1中放入消息 */
		/* sem_wait(mutex1); */
		sem_wait(empty1);
		printf("进程0向信箱M1中放入一条消息\n");
		/* sem_post(mutex1); */
		sem_post(full1);
		/* sleep(1); */
	}
	
}
//进程2
void *process2(void *arg)
{
	int i=20;
	while(i--)
	{
		//从M1中取出消息
		/* sem_wait(mutex1); */
		sem_wait(full1);
		printf("进程1从信箱M1中取出一条消息\t");
		/* sem_post(mutex1); */
		sem_post(empty1);
		//向M2中放入消息
		/* sem_wait(mutex2); */
		sem_wait(empty2);
		printf("进程1向信箱M2中放入一条消息\n");
		/* sem_post(mutex2); */
		sem_post(full2);
	}
	
}
//进程3
void *process3(void *arg)
{
	int i=20;
	while(i--)
	{
		//从M2中取出消息
		/* sem_wait(mutex2); */
		sem_wait(full2);
		printf("进程2从信箱M2中取出一条消息\t");
		/* sem_post(mutex2); */
		sem_post(empty2);
		//向M3中放入消息
		/* sem_wait(mutex3); */
		sem_wait(empty3);
		printf("进程2向信箱M3中放入一条消息\n");
		/* sem_post(mutex3); */
		sem_post(full3);
		sleep(1);
	}
	
}
//进程4
void *process4(void *arg)
{
	int i=20;
	while(i--)
	{
		//从M3中取出消息
		/* sem_wait(mutex3); */
		sem_wait(full3);
		printf("进程3从信箱M3中取出一条消息\t");
		/* sem_post(mutex3); */
		sem_post(empty3);
		//向M0中放入消息
		/* sem_wait(mutex0); */
		sem_wait(empty0);
		printf("进程3向信箱M0中放入一条消息\n");
		/* sem_post(mutex0); */
		sem_post(full0);
		sleep(1);
	}
	
}
int main(int argc,char *argv[])
{
	//创建信号量
	/* mutex0=sem_open("mutex0",O_CREAT,0666,1); */
	/* mutex1=sem_open("mutex1",O_CREAT,0666,1); */
	/* mutex2=sem_open("mutex2",O_CREAT,0666,1); */
	/* mutex3=sem_open("mutex3",O_CREAT,0666,1); */
	empty0=sem_open("empty00",O_CREAT,0666,0);
	empty1=sem_open("empty11",O_CREAT,0666,3);
	empty2=sem_open("empty22",O_CREAT,0666,2);
	empty3=sem_open("empty33",O_CREAT,0666,2);
	full0=sem_open("full00",O_CREAT,0666,3);
	full1=sem_open("full11",O_CREAT,0666,0);
	full2=sem_open("full22",O_CREAT,0666,0);
	full3=sem_open("full33",O_CREAT,0666,0);
	pthread_t p1,p2,p3,p4;
	pthread_create(&p1,NULL,process1,NULL);
	pthread_create(&p2,NULL,process2,NULL);
	pthread_create(&p3,NULL,process3,NULL);
	pthread_create(&p4,NULL,process4,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(p3,NULL);
	pthread_join(p4,NULL);
	/* sem_close(mutex0); */
	/* sem_close(mutex1); */
	/* sem_close(mutex2); */
	/* sem_close(mutex3); */
	sem_close(empty0);
	sem_close(empty1);
	sem_close(empty2);
	sem_close(empty3);
	sem_close(full0);
	sem_close(full1);
	sem_close(full2);
	sem_close(full3);
	/* sem_unlink("mutex0"); */
	/* sem_unlink("mutex1"); */
	/* sem_unlink("mutex2"); */
	/* sem_unlink("mutex3"); */
	sem_unlink("empty00");
	sem_unlink("empty11");
	sem_unlink("empty22");
	sem_unlink("empty33");
	sem_unlink("full00");
	sem_unlink("full11");
	sem_unlink("full22");
	sem_unlink("full33");
	return 0;
}