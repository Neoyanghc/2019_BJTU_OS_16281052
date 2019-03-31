#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

sem_t *p1_signal = NULL;
sem_t *p2_signal = NULL;
sem_t *p3_signal = NULL;

int main(int argc, char *argv[])
{
    pid_t pid;
    p1_signal=sem_open("P1_signalname",O_CREAT,0666,0);
	p2_signal=sem_open("P2_signalname",O_CREAT,0666,0);
	p3_signal=sem_open("P3_signalname",O_CREAT,0666,0);
    pid = fork();

    if (pid < 0)
    { // 没有创建成功
        perror("fork create error");
    }
    if (0 == pid)
    { // 子进程
        
        sem_wait(p1_signal);
        printf("I am the Process P2\n");
        sem_post(p1_signal);
        sem_post(p2_signal);
    }
    else if (pid > 0)
    { // 父进程
        printf("I am the Process P1\n");
        sem_post(p1_signal);
        pid = fork();
        if (pid < 0)
        { // 没有创建成功
            perror("fork create error");
        }
        if (0 == pid)
        { // 子进程
            sem_wait(p1_signal);
            printf("I am the Process P3\n");
            sem_post(p1_signal);
            sem_post(p3_signal);
            pid = fork();
            if (pid < 0)
            { // 没有创建成功
                perror("fork create error");
            }
            if (0 == pid)
            { // 子进程
                sem_wait(p2_signal);
                sem_wait(p3_signal);
                printf("I am the Process P4\n");
            }
        }
    }
    sem_close(p1_signal);
    sem_unlink("p1_signalname");
    sem_close(p2_signal);
    sem_unlink("p2_signalname");
    sem_close(p3_signal);
    sem_unlink("p3_signalname");
    return 0;
}
