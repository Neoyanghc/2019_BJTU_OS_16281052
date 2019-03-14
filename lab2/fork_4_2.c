#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
    int i = 0;
    pid_t fpid,ppid,ppid2;
    //pid指当前进程的pid,
    //fpid指fork返回给当前进程的值
    ppid = getppid();
    ppid2 = getpid();
    printf("the first node of tree‘s father %d\n",ppid);
    for (i = 0; i < 2; i++)
    {
        fpid = fork();
        if (fpid == 0)
        {
            // printf("%d child  %4d %4d %4d\n", i, getppid(), getpid(), fpid);
        }
        else{
            printf("%d parent %4d %4d\n", i, getpid(), fpid);
        }
        if (i == 1 && fpid != 0 && ppid != getppid())
        {
            fpid = fork();
            if (fpid == 0)
            {
                // printf("%d child  %4d %4d %4d\n", i, getppid(), getpid(), fpid);
            }
            else{
                 printf("%d parent %4d %4d\n", i,getpid(), fpid);
            }
        }
    }
    i = 10;
    while(i-->0)
    {
         printf("%4d parent %4d\n", getppid(), getpid());
         sleep(3);
         if(ppid2+1 == getpid()&&i<7)
         {
             exit(1);
         }
    }
    return 0;
}