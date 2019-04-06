#include <stdio.h>                  
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

const unsigned int PRODUCER_COUNT = 5;	//生产者个数
const unsigned int CONSUMER_COUNT = 3;	//消费者个数
pthread_mutex_t m_mutex;
pthread_cond_t  m_cond;
//缓冲区 
int nReady = 0;
//消费者
void *consumer(void *args)
{
    int id = *(int *)args;
    delete (int *)args;
    while (true)
    {
        pthread_mutex_lock(&m_mutex);    //锁定mutex
        while (!(nReady > 0))
        {
            printf("-- thread %d wait...\n", id);
            pthread_cond_wait(&m_cond, &m_mutex);    //等待条件变量
        }
 
        printf("** thread %d alive, and consume product %d ...\n", id, nReady);
        -- nReady;  //消费
        printf("   thread %d end consume... \n\n", id);
 
        pthread_mutex_unlock(&m_mutex);  //解锁mutex
        sleep(1);
    }
    pthread_exit(NULL);
}
 
//生产者
void *producer(void *args)
{
    int id = *(int *)args;
    delete (int *)args;
    while (true)
    {
        pthread_mutex_lock(&m_mutex);    //锁定mutex
 
        printf("++ thread %d signal, and produce product %d ...\n", id, nReady+1);
        ++ nReady;      //生产
        pthread_cond_signal(&m_cond);  //发送条件变量信号
        printf("   thread %d end produce, signal...\n\n", id);
        pthread_mutex_unlock(&m_mutex); //解锁mutex
        sleep(1);
    }
    pthread_exit(NULL);
}
 
int main()
{
    pthread_t thread[PRODUCER_COUNT+CONSUMER_COUNT];
 
	    //初始化互斥量
	 pthread_mutex_init(&m_mutex, NULL);
	  //初始化条件变量
	   pthread_cond_init(&m_cond, NULL);
    //首先生成消费者
    for (unsigned int i = 0; i < CONSUMER_COUNT; ++i)
        pthread_create(&thread[i], NULL, consumer, new int(i));
    sleep(1);   //使生产者等待一段时间, 加速消费者等待事件产生
    //然后生成生产者
    for (unsigned int i = 0; i < PRODUCER_COUNT; ++i)
        pthread_create(&thread[CONSUMER_COUNT+i], NULL, producer, new int(i));
    for (unsigned int i = 0; i < PRODUCER_COUNT+CONSUMER_COUNT; ++i)
        pthread_join(thread[i], NULL);
	 pthread_mutex_destroy(&m_mutex);
	//销毁条件变量
	  pthread_cond_destroy(&m_cond);
}


